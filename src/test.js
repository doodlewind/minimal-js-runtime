import { fib } from 'fib.so'
import { setTimeout } from 'os'

console.log(`fib(10) = ${fib(10)}`)
setTimeout(() => console.log('World!'), 1000)
console.log('Hello')
