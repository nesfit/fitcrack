<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container>
    <!--Show popups when clicked-->
    <functionInsertPopup v-bind:functionsInsertPopup="functionsInsertPopup" v-on:hide-insert-popup="hideInsertPopup"
      v-on:update-rule="updateRule">
    </functionInsertPopup>
    <allFunctionsPopup v-bind:allFunctionsPopup="allFunctionsPopup" v-on:hide-all-functions-popup="hideAllFunctionsPopup"
      v-on:show-insert-popup="showInsertPopup"></allFunctionsPopup>
    <v-row justify="center">
      <mainEditWindow v-bind:rules="rules" v-bind:editingFile="editingFile" v-bind:ruleFileInfo="ruleFileInfo"
        v-on:update-rules="updateRules" v-on:show-insert-popup="showInsertPopup"
        v-on:show-all-functions-popup="showAllFunctionsPopup"></mainEditWindow>
      <liveKeyspacePreview v-bind:allPasswordsString="allPasswordsString" v-bind:previewPasswords="previewPasswords"
        v-on:update-passwords="updatePasswords">
      </liveKeyspacePreview>
    </v-row>
  </v-container>
</template>

  
<script>
import Vue from 'vue';
import { debounce } from 'lodash';
import functionsJson from "@/assets/ruleFunctions.json"
import mainEditWindow from "@/components/rule/mainEditWindow/mainEditWindow.vue";
import liveKeyspacePreview from "@/components/rule/livePreview/liveKeyspacePreview.vue"
import functionInsertPopup from "@/components/rule/mainEditWindow/popups/functionInsertPopup.vue"
import allFunctionsPopup from '@/components/rule/mainEditWindow/popups/allFunctionsPopup.vue';


export default {
  data() {
    return {
      showConfirmation: false,
      ruleFunctions: functionsJson,
      backupRules: [{ value: "", error: false }],
      rules: [{ value: "", error: false }],
      allPasswordsString: "p@ssW0rd",
      passwordsList: [],
      applicatorResult: [],
      previewPasswords: {
        string: "",
        loading: false
      },
      functionsInsertPopup: {
        visible: false,
        functionIndex: 0,
        ruleIndex: 0
      },
      allFunctionsPopup: {
        visible: false,
        onlyShow: true,
        ruleIndex: 0
      },
      editingFile: false,
      ruleFileInfo: {}
    };
  },
  methods: {
    updateRules(updatedRules, emptyRuleAdded) {
      this.rules = updatedRules;
      // generate preview always except when empty rule was added
      if (!emptyRuleAdded) {
        this.generatePreview();
      }
    },
    updateRule(functionToAdd) {
      //update the rule, where rule function was inserted
      this.rules[this.functionsInsertPopup.ruleIndex].value = this.rules[this.functionsInsertPopup.ruleIndex].value.trimEnd().concat(functionToAdd);
      //Vue.set(this.rules, this.functionsInsertPopup.ruleIndex, this.rules[this.functionsInsertPopup.ruleIndex].trimEnd().concat(functionToAdd)); TODO
      //get cursor position
      console.log(this.$refs)
      //const inputElement = this.$refs["rule-" + this.functionsInsertPopup.ruleIndex].$el.querySelector('input');
      //const cursorPosition = inputElement.selectionStart;

      this.generatePreview();
    },
    updatePasswords(updatedAllPasswordsString) {
      this.allPasswordsString = updatedAllPasswordsString;
      this.generatePreview();
    },
    /**
     * Function which generates password preview, gets mangled passwords
     */
    generatePreview() {
      this.previewPasswords.loading = true; //Show loading button
      this.passwordsList = this.allPasswordsString.split("\n"); //create an array of passwords
      const data = {
        rules: this.rules.map(rule => rule.value),
        passwordsList: this.passwordsList
      };

      // Get the result
      this.axios.post(this.$serverAddr + "/rule/preview", data).then((response) => {
        this.applicatorResult = response.data.passwordsPreview;
        console.log(response.data.passwordsPreview)
        this.filterPreviewPasswords();
      }).catch((error) => {
        //TODO this.$error
        this.previewPasswords.string = "Could not mangle the passwords."
      }).finally(() => {
        this.previewPasswords.loading = false; //the loading is done
      })
    },
    /**
     * Create a string from all correctly mangled passwords and mark if rule is with error
     */
    filterPreviewPasswords() {
      let previewPasswordsList = []; // list for storing only mangled passwords with no error
      let ruleIndex = 0;

      this.applicatorResult.forEach((element) => {
        if (ruleIndex < this.rules.length) {
          if (element.retCode == -1) {
            this.rules[ruleIndex].error = true;
          }
          else {
            this.rules[ruleIndex].error = false;
          }
          ruleIndex += 1;
        }
        if (element.retCode >= 0) {
          previewPasswordsList.push(element.finalPassword);
        }
      });

      this.previewPasswords.string = previewPasswordsList.join("\n");
    },
    showInsertPopup(insertData) {
      this.functionsInsertPopup = insertData;
      if (this.ruleFunctions[insertData.functionIndex].operands.length == 0) { //if the inserted function has no operands, insert the function immediatelly
        Vue.set(this.functionsInsertPopup, 'visible', false);
        const ruleFunction = " " + this.ruleFunctions[insertData.functionIndex].sign;
        this.updateRule(ruleFunction)
      }
      else { //otherwise show the insert popup
        Vue.set(this.functionsInsertPopup, 'visible', true);
      }
      console.log(this.functionsInsertPopup)

    },
    hideInsertPopup(visibility) {
      console.log(this.functionsInsertPopup)
      this.functionsInsertPopup.visible = visibility;
    },
    showAllFunctionsPopup(popupData) {
      this.allFunctionsPopup = popupData;
    },
    hideAllFunctionsPopup(visibility) {
      this.allFunctionsPopup.visible = visibility;
    },
    loadRuleFile() {
      // get the content of the rule file
      this.axios.get(this.$serverAddr + '/rule/' + this.$route.params.id + '/download').then((response) => {
        this.editingFile = true; // mark that file is edited, not created
        // get the information about rule file
        this.axios.get(this.$serverAddr + '/rule/' + this.$route.params.id).then((response) => {
          this.ruleFileInfo = response.data;
        })
        const lines = response.data.split("\n")
        this.rules = lines.map(line => ({ value: line, error: false }))
        this.backupRules = this.rules.slice()
        this.generatePreview(); //generate the initial mangled passwords
      }).catch((error) => { // handle specifying wrong rule file ID in URL
        this.$error('Rule file with such ID does not exist.')
        this.$router.push({ name: 'ruleFileCreate' });
      })
    },
    /**
     * Function which checks if the rules have been changed compared to beginning
     */
    rulesChanged() {
      return this.backupRules.map(rule => rule.value).toString() !== this.rules.map(rule => rule.value).toString();
    },
    /**
     * Function which shows confirmation window when reload or go back button is clicked
     */
    navigateFromPage(event) {
      if (this.rulesChanged() && !this.showConfirmation) {
        event.preventDefault();
        event.returnValue = '';
      }
    }
  },
  mounted() {
    if (this.$route.params.id) { //when there is id parameter in route url
      this.loadRuleFile();
    }
    this.generatePreview = debounce(this.generatePreview, 2000); // call generatePreview only 2 seconds after rule change (because of application performance, prevents too many requests)
  },
  beforeMount() {
    window.addEventListener('beforeunload', this.navigateFromPage);
  },
  beforeDestroy() {
    window.removeEventListener('beforeunload', this.navigateFromPage);
  },
  destroyed(){
    this.generatePreview.cancel()
  },
  /**
   * Function which shows confirm message when route is changed
   */
  beforeRouteLeave(to, from, next) {
    if (this.rulesChanged() && !to.params.skipConfirmWindow) {
      if (confirm("Changes you made may not be saved.")) {
        this.showConfirmation = true;
        next();
      }
      else {
        next(false);
      }
    }
    else {
      next();
    }
  },
  components: { mainEditWindow, liveKeyspacePreview, functionInsertPopup, allFunctionsPopup },
};
</script>


<style></style>