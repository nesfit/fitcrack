<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <div>
      <VuePellEditor
        :actions="editorOptions"
        :content="editorContent"
        :placeholder="editorPlaceholder"
        v-model="editorContent"
        :style-with-css="true"
        default-paragraph-separator="span"
        @change="doSomething"
        @mounted="doSomethingAfterMounted"
      />
      {{editorContent}}
    </div>
</template>

<script>
  import VuePellEditor from 'vue-pell-editor'
  import maskSymbol from '@/components/mask/maskSymbol'
  export default {
    name: "maskEditor",
    data: () => ({
      editorContent: '',
      editorOptions: [
        'bold',
        'underline',
        {
          name: 'italic',
          result: () => window.pell.exec('italic')
        },
        {
          name: 'custom',
          icon: '[a-z]',
          title: 'Custom Action',
          result: () => window.pell.exec('insertHTML', '<span symbol="?l" contenteditable="false">aafsdfa</span>')
        },
        {
          name: 'image',
          result: () => {
            const url = window.prompt('Enter the image URL')
            if (url) window.pell.exec('insertImage', ensureHTTP(url))
          }
        },
        {
          name: 'link',
          result: () => {
            const url = window.prompt('Enter the link URL')
            if (url) window.pell.exec('createLink', ensureHTTP(url))
          }
        }
      ],
      editorPlaceholder: 'Write something amazing...',
      editorContent: 'Predefined Content'
    }),
    components: {
      VuePellEditor,
      maskSymbol
    },
    methods: {
      doSomething() {
        console.log('Hello')
      },
      doSomethingAfterMounted() {
        console.log('Editor mounted')
      }
    }
  }
</script>

<style >
  .maskSymbol {
    margin-left: 2px;
    margin-right: 2px;
    border: 2px solid;
    color: white;
    border-radius: 4px;
  }
</style>
