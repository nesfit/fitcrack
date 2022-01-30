import { defineConfig } from 'vite'
import { createVuePlugin } from 'vite-plugin-vue2'
const path = require('path')

// https://vitejs.dev/config/
export default defineConfig({
  resolve: {
    alias:{
      '@' : path.resolve(__dirname, './src')
    }
  },
  plugins: [createVuePlugin()]
})
