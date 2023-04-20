<!--
   * Author : Jiří Mládek
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container>
    <!-- Show popups when clicked -->
    <functionInsertPopup v-bind:functionsInsertPopup="functionsInsertPopup" v-on:hide-insert-popup="hideInsertPopup"
      v-on:add-function="addFunction">
    </functionInsertPopup>
    <allFunctionsPopup v-bind:allFunctionsPopup="allFunctionsPopup" v-on:hide-all-functions-popup="hideAllFunctionsPopup"
      v-on:show-insert-popup="showInsertPopup"></allFunctionsPopup>

    <!-- Tool has 2 parts, main window for editing rules and preview of mangled passwords -->
    <v-row justify="center">
      <mainEditWindow v-bind:rules="rules" v-bind:editingFile="editingFile" v-bind:ruleFileInfo="ruleFileInfo"
        v-on:update-rules="updateRules" v-on:show-insert-popup="showInsertPopup"
        v-on:show-all-functions-popup="showAllFunctionsPopup"></mainEditWindow>
      <liveKeyspacePreview v-bind:allPasswordsString="allPasswordsString" v-bind:mangledPasswords="mangledPasswords"
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
      ruleFunctions: functionsJson, // array for storing rule functions objects
      backupRules: [{ value: "", error: false }], // array of objects for storing initial state of rules
      rules: [{ value: "", error: false }], // array of objects for storing rules
      allPasswordsString: "p@ssW0rd\n", // string for storing all passwords concatenated
      applicatorResult: [], // array for storing result from rules applicator (response from server)
      mangledPasswords: { 
        value: "", // string with all mangled passwords concatenated
        loading: false // boolean to mark if mangled passwords are loading from server
      },
      functionsInsertPopup: {
        visible: false, 
        functionIndex: 0, // index of function in Json array
        ruleIndex: 0, // index of rule, to which function will be inserted
        cursorPosition: 0 // cursor position in edited rule
      },
      allFunctionsPopup: {
        visible: false,
        onlyShow: true, // true if functions in popup are not clickable, false if functions can be selected
        ruleIndex: 0, // index of rule, to which function will be inserted
        cursorPosition: 0 // cursor position in edited rule
      },
      editingFile: false, // false if creating file, true if editing file
      ruleFileInfo: {} // object for storing information about edited rule file
    };
  },
  methods: {
    /**
     * Method which updates rules received from child components
     * @param {Array} updatedRules Updated array of rules
     * @param {Boolean} emptyRuleAdded True if empty rule was added
     */
    updateRules(updatedRules, emptyRuleAdded) {
      this.rules = updatedRules;
      // generate preview always except when empty rule was added
      if (!emptyRuleAdded) {
        this.mangledPasswords.loading = true;
        this.mangledPasswords.value = "";
        this.generatePreview();
      }
    },
    /**
     * Method which updates passwords received from child components
     * @param {String} updatedAllPasswordsString Updated array of rules
     */
    updatePasswords(updatedAllPasswordsString) {
      this.allPasswordsString = updatedAllPasswordsString;
      this.mangledPasswords.loading = true;
      this.mangledPasswords.value = "";
      this.generatePreview(); 
    },
        /**
     * Method which adds a rule function to an existing specific rule 
     * @param {String} functionToAdd Rule function created from insert popup
     */
     addFunction(functionToAdd) {
      //update the specific rule, add created function
      const currentRule = this.rules[this.functionsInsertPopup.ruleIndex].value;
      const cursorPosition = this.functionsInsertPopup.cursorPosition;
      const firstPart = currentRule.slice(0, cursorPosition);
      const secondPart = currentRule.slice(cursorPosition);
      let finalRule = firstPart; 

      // check if space should be added before the inserted function
      if(cursorPosition !== 0){
        if (firstPart.slice(-1) !== ' ') {
          finalRule = finalRule.concat(" ");
        }
      }
      finalRule = finalRule.concat(functionToAdd); // add inserted function to the final rule

      // check if space should be added after the inserted function
      if (secondPart.charAt(0) !== ' ' && secondPart.charAt(0) !== '') {
        finalRule = finalRule.concat(" ");
      }
      finalRule = finalRule.concat(secondPart); // concatenate with the second part
      this.rules[this.functionsInsertPopup.ruleIndex].value = finalRule;

      //generate mangled passwords preview
      this.mangledPasswords.loading = true;
      this.mangledPasswords.value = "";
      this.generatePreview(); 
    },
    /**
     * Method which generates passwords preview, gets mangled passwords
     */
    generatePreview() {
      this.mangledPasswords.loading = true; // Show loading button until generating is done
      let passwordsList = this.allPasswordsString.split("\n"); // create an array of passwords
      const data = {
        rules: this.rules.map(rule => rule.value),
        passwordsList: passwordsList
      };
      // Get the result of rule applicator
      this.axios.post(this.$serverAddr + "/rule/preview", data).then((response) => {
        this.applicatorResult = response.data.passwordsPreview;
        this.filterPasswordsPreview();
      }).catch((error) => {
        this.$error("Could not mangle the passwords.")
        this.mangledPasswords.value = "";
      }).finally(() => {
        this.mangledPasswords.loading = false; // Loading is done
      })
    },
    /**
     * Create a string from all correctly mangled passwords and mark if rule is with error
     */
    filterPasswordsPreview() {
      let mangledPasswordsList = []; // array for storing only mangled passwords with no error
      let ruleIndex = 0;

      // go through each mangled password from rule applicator
      this.applicatorResult.forEach((element) => {
        // mark if rule has an error
        if (ruleIndex < this.rules.length) {
          if (element.retCode == -1) {
            this.rules[ruleIndex].error = true;
          }
          else {
            this.rules[ruleIndex].error = false;
          }
          ruleIndex += 1;
        }
        // if there was no error while mangling, add password to array
        if (element.retCode >= 0) {
          mangledPasswordsList.push(element.finalPassword);
        }
      });
      this.mangledPasswords.value = mangledPasswordsList.join("\n"); // create a string from all correct mangled passwords
    },
    /**
     * Method which takes care of showing the Insert popup
     * @param {Object} popupData data for Insert popup to be updated 
     */
    showInsertPopup(popupData) {
      this.functionsInsertPopup = popupData;
      if (this.ruleFunctions[popupData.functionIndex].operands.length == 0) { // if the inserted function has no operands, insert the function immediatelly
        Vue.set(this.functionsInsertPopup, 'visible', false); // popup is not going to show up
        const ruleFunction = this.ruleFunctions[popupData.functionIndex].sign;
        this.addFunction(ruleFunction)
      }
      else { //otherwise show the insert popup
        Vue.set(this.functionsInsertPopup, 'visible', true);
      }
    },
    /**
     * Method which hides the Insert popup
     */
    hideInsertPopup() {
      this.functionsInsertPopup.visible = false;
    },
    /**
     * Method which takes care of showing the All funtions popup
     * @param {Object} popupData data for All functions popup to be updated 
     */
    showAllFunctionsPopup(popupData) {
      this.allFunctionsPopup = popupData;
    },
    /**
     * Method which hides the All functions popup
     */
    hideAllFunctionsPopup() {
      this.allFunctionsPopup.visible = false;
    },
    /**
     * Method which loads the existing rule file from server (only when editing file)
     */
    loadRuleFile() {
      // get the content of the rule file
      this.axios.get(this.$serverAddr + '/rule/' + this.$route.params.id + '/download').then((response) => {
        this.editingFile = true; // mark that file is edited, not created
        // get the information about rule file
        this.axios.get(this.$serverAddr + '/rule/' + this.$route.params.id).then((response) => {
          this.ruleFileInfo = response.data;
        });
        // store the rules 
        const lines = response.data.split("\n")
        this.rules = lines.map(line => ({ value: line, error: false }))
        this.backupRules = JSON.parse(JSON.stringify(this.rules)); //keep a copy of initial rules
        this.generatePreview(); //generate the initial mangled passwords
      }).catch((error) => {
        // handle specifying wrong rule file ID in URL and redirect to creating file
        this.$error('Rule file with such ID does not exist.')
        this.$router.push({ name: 'ruleFileCreate' });
      });
    },
    /**
     * Method which checks if the rules have been changed compared to beginning (for redirecting off page, so that changes were not lost)
     */
    rulesChanged() {
      return this.backupRules.map(rule => rule.value).toString() !== this.rules.map(rule => rule.value).toString();
    },
    /**
     * Method which shows confirmation window when reload button is clicked
     */
    navigateFromPage(event) {
      if (this.rulesChanged()) {
        event.preventDefault();
        event.returnValue = '';
      }
    }
  },
  mounted() {
    if (this.$route.params.id) { // when there is id parameter in route url, load the existing rule file
      this.mangledPasswords.loading = true;
      this.loadRuleFile();
    }
    /* call generatePreview 2 seconds after rule change or password change
       because of application performance, prevents too many requests) */
    this.generatePreview = debounce(this.generatePreview, 1500); 
  },
  beforeMount() {
    window.addEventListener('beforeunload', this.navigateFromPage); // event listener for navigating off page (reload button)
  },
  beforeDestroy() {
    window.removeEventListener('beforeunload', this.navigateFromPage);
  },
  destroyed() { 
    this.generatePreview.cancel() // cancel the debounce method
  },
  /**
   * Method which shows confirm message when route is changed
   */
  beforeRouteLeave(to, from, next) {
    // when rules were changed compared to initial and when confirm window should not be skipped, then show confirm message
    if (this.rulesChanged() && !to.params.skipConfirmWindow) {
      if (confirm("Changes you made may not be saved.")) {
        next(); // redirect
      }
      else {
        next(false);
      }
    }
    else {
      next(); // redirect
    }
  },
  components: {
    mainEditWindow,
    liveKeyspacePreview,
    functionInsertPopup,
    allFunctionsPopup
  },
};
</script>


<style></style>