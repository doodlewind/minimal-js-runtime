import { setTimeout } from 'uv'

setTimeout(() => console.log('B'), 0)
Promise.resolve().then(() => console.log('A'))

setTimeout(() => {
  setTimeout(() => console.log('D'), 0)
  Promise.resolve().then(() => console.log('C'))
}, 1000)
