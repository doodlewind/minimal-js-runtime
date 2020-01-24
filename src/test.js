import { fib } from 'fib.so'
import { setTimeout } from 'uv'

console.log(`fib(10) = ${fib(10)}`)
setTimeout(() => console.log('World!'), 1000)
console.log('Hello')
