<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="d-flex justify-center">
    <v-row>
      <v-col cols="12" md="6" class="bordered">
        <input type="file" label="Load Dictionary" @change="onRuleFileChange($event)">
        <v-textarea label="Rule File Content" v-model="ruleContent" readonly></v-textarea>
      </v-col>
      <v-col cols="12" md="4" class="bordered">
        <input type="file" label="Load Dictionary" @change="onDictionaryFileChange($event)">
        <v-textarea label="Dictionary Content" v-model="dictionaryContent" readonly></v-textarea>
        <v-btn @click="generatePreview">Generate Preview</v-btn>
        <v-textarea label="Preview Passwords" v-model="previewPasswords" readonly></v-textarea>
       </v-col>
    </v-row>
  </v-container>
</template>
  

  
<script>
  export default {
    data() {
      return {
        ruleFile: null,
        ruleContent: null,
        dictionaryFile: null,
        dictionaryContent: null,
        previewPasswords: null,
      };
    },
    methods: {
        onRuleFileChange(event) {
          const file = event.target.files[0];
            const reader = new FileReader();
            reader.readAsText(file);
            reader.onload = (event) => {
                this.ruleContent = event.target.result.split("\n")
            };
            },
        onDictionaryFileChange(event) {
            const file = event.target.files[0];
            const reader = new FileReader();
            reader.readAsText(file);
            reader.onload = (event) => {
                this.dictionaryContent = event.target.result.split("\n")
            };
            },
      generatePreview() {
        const data = {
          ruleContent: this.ruleContent,
          dictionaryContent: this.dictionaryContent,
        };
        const changedData = data
        console.log(data)
        this.axios.post(this.$serverAddr + '/rule/rule/preview', changedData).then((response) => {
          this.previewPasswords = response.data;
          console.log(this.previewPasswords)
        }).catch((error) => {
          this.previewPasswords = error.message;
        });
      },
    },
  };
</script>


<style>
.bordered {
  border: 1px solid #ccc;
  border-top-right-radius: 4px;
  border-bottom-right-radius: 4px;
  border-top-left-radius: 4px;
  border-bottom-left-radius: 4px;
  margin: 5px
}


</style>