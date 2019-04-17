const { wait } = require('./index');
const https = require('https');

//let a;
//wait(
//    () =>
//        new Promise(resolve =>
//            https.get('https://randomuser.me/api', res => {
//                a = res.headers;
//                resolve();
//            })
//        )
//);
//console.log(a);
 for (let i = 1; i < 10; ++i) {
   wait(
     () =>
       new Promise(resolve => {
         setTimeout(() => {
           console.log(4);
           resolve();
         }, 100);
       })
   );
 }
 console.log(5);
