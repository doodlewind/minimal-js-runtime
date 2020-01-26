import { fib } from 'fib.so'
import { setTimeout } from 'uv'

console.log(`fib(10) = ${fib(10)}`)

console.log('Hello')

setTimeout(() => console.log('!'), 0)

Promise.resolve().then(() => console.log('World'))
