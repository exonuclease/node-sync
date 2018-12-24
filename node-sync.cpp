#include "napi.h"
#include "uv.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace node_sync
{
using namespace Napi;
using std::cout;
using std::endl;
using std::find_first_of;
using std::for_each;
using std::make_unique;
using std::mismatch;
using std::unique_ptr;
using std::vector;

unique_ptr<vector<void *>> get_handle_id(uv_loop_t *loop, unique_ptr<vector<void *>> &&vec)
{
  vec->clear();
  auto vecp = vec.release();
  uv_walk(loop, [](uv_handle_t *handle, void *arg) {
    auto vecp = static_cast<vector<void *> *>(arg);
    vecp->push_back(uv_handle_get_data(handle));
  },
          vecp);
  return unique_ptr<vector<void *>>(vecp);
}

Value Method(const CallbackInfo &info)
{
  Env env = info.Env();
  HandleScope scope(env);
  uv_loop_t *loop;
  NAPI_EXTERN napi_get_uv_event_loop(env, &loop);
  auto cb = info[0].As<Function>();
  auto handles_ids1 = make_unique<vector<void *>>();
  auto handles_ids2 = make_unique<vector<void *>>();
  handles_ids1 = get_handle_id(loop, std::move(handles_ids1));
  auto result = cb.Call(info.This(), {});
  handles_ids2 = get_handle_id(loop, std::move(handles_ids2));
  auto diff_position = mismatch(handles_ids1->cbegin(), handles_ids1->cend(), handles_ids2->cbegin(), handles_ids2->cend());
  vector<void *> diff_pointers(diff_position.second, handles_ids2->cend());
  bool flag = true;
  while (flag)
  {
    handles_ids1 = get_handle_id(loop, std::move(handles_ids1));
    if (find_first_of(handles_ids1->cbegin(), handles_ids1->cend(), diff_pointers.cbegin(), diff_pointers.cend()) == handles_ids1->cend())
    {
      flag = false;
    }
    env.Global().Get("process").As<Object>().Get("_tickCallback").As<Function>().Call(env.Global(), {});
    uv_run(loop, UV_RUN_ONCE);
  }
  uv_run(loop, UV_RUN_DEFAULT);
  return result;
}

static Object Init(Env env, Object exports)
{
  exports.Set(Napi::String::New(env, "wait"), Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(node_sync, Init)
} // namespace node_sync
