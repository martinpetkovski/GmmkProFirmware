const { compile } = require('nexe')

compile({
  input: './core.js',
  build: true, //required to use patches
}).then(() => {
  console.log('success')
})