<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="d-flex justify-center">
    <v-row>
      <v-col cols="12" md="6" class="bordered">
        <input type="file" label="Load Dictionary" @change="onRuleFileChange($event)">
        <v-textarea label="Rule File Content" v-model="rulesContent"></v-textarea>
      </v-col>
      <v-col cols="12" md="4" class="bordered">
        <input type="file" label="Load Dictionary" @change="onDictionaryFileChange($event)">
        <v-textarea label="Dictionary Content" v-model="dictionaryContent"></v-textarea>
        <v-btn @click="generatePreview">Generate Preview</v-btn>
        <v-textarea label="Preview Passwords" v-model="previewPasswordsString"></v-textarea>
      </v-col>
      <v-col>
        <v-btn @click="generateRandomRule">Generate random rule</v-btn>
      </v-col>

    </v-row>
  </v-container>
</template>
  

  
<script>
export default {
  data() {
    return {
      rulesContent: null,
      dictionaryContent: null,
      rulesList: null,
      passwordsList: null,
      applicatorResult: null,
      previewPasswords: [],
      previewPasswordsString: "",
      minFunctionsNum: 6,
      maxFunctionsNum: 8,
      randomRuleString: ""
    };
  },
  methods: {
    onRuleFileChange(event) {
      const file = event.target.files[0];
      const reader = new FileReader();
      reader.onload = (event) => {
        this.rulesContent = event.target.result
        this.rulesList = event.target.result.split("\n")
        this.rulesList.pop()
      };
      reader.readAsText(file);
    },
    onDictionaryFileChange(event) {
      const file = event.target.files[0];
      const reader = new FileReader();
      reader.onload = (event) => {
        this.dictionaryContent = event.target.result
        this.passwordsList = event.target.result.split("\n")
        this.passwordsList.pop()
      };
      reader.readAsText(file);
    },
    generatePreview() {
      const data = {
        rulesList: this.rulesList,
        passwordsList: this.passwordsList
      };
      this.axios.post(this.$serverAddr + '/rule/preview', data).then((response) => {
        this.applicatorResult = response.data.passwordsPreview;
        this.filterPreviewPasswords()
      }).catch((error) => {
        this.previewPasswords = error.message;
      });
    },
    generateRandomRule() {
      const data = {
        minFunctionsNum: this.minFunctionsNum,
        maxFunctionsNum: this.maxFunctionsNum
      }
      this.axios.post(this.$serverAddr + "/rule/randomRule", data).then((response) =>{
        this.randomRuleString = response.data.randomRule;
        console.log(this.randomRuleString)
      }).catch((error) => {
        this.randomRuleString = error.message;
      });
    },
    filterPreviewPasswords() {
      this.previewPasswords = []
      this.applicatorResult.forEach(element => {
        if (element.retCode >= 0) {
          this.previewPasswords.push(element.finalPassword)
        }
      });
      this.previewPasswordsString = this.previewPasswords.join("\n")
      console.log(this.previewPasswords)
    }
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