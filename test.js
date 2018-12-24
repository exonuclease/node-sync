const { wait } = require('./index');
const https = require('https');

console.log(1);
wait(
  () =>
    new Promise(resolve =>
      https.get('https://randomuser.me/api', () => {
        console.log(2);
        resolve();
      })
    )
);
console.log(3);
// for (let i = 1; i < 10; ++i) {
//   wait(
//     () =>
//       new Promise(resolve => {
//         setTimeout(() => {
//           console.log(4);
//           resolve();
//         }, 5);
//       })
//   );
// }
// console.log(5);
