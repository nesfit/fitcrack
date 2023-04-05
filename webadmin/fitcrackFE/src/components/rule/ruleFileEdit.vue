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
      <mainEditWindow v-bind:rulesList="rulesList" v-bind:editingFile="editingFile" v-bind:ruleFileInfo="ruleFileInfo"
        v-on:update-rules="updateRules" v-on:show-insert-popup="showInsertPopup"
        v-on:show-all-functions-popup="showAllFunctionsPopup"></mainEditWindow>
      <liveKeyspacePreview v-bind:previewPasswords="previewPasswords" v-on:generate-preview="generatePreview">
      </liveKeyspacePreview>
    </v-row>
  </v-container>
</template>

  
<script>
import Vue from 'vue';
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
      backupRulesList: [""],
      rulesList: [""],
      passwordsList: [""],
      applicatorResult: "",
      previewPasswordsList: [],
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
    updateRules(updatedRulesList) {
      this.rulesList = updatedRulesList;
    },
    updateRule(functionToAdd) {
      //update the rule, where rule function was inserted
      Vue.set(this.rulesList, this.functionsInsertPopup.ruleIndex, this.rulesList[this.functionsInsertPopup.ruleIndex].trimEnd().concat(functionToAdd));
    },
    /**
     * Function which generates password preview, gets mangled passwords
     */
    generatePreview(passwordsContent) {
      this.previewPasswords.loading = true; //Show loading button

      this.passwordsList = passwordsContent.split("\n");
      const data = {
        rulesList: this.rulesList,
        passwordsList: this.passwordsList
      };
      // Get the result
      this.axios.post(this.$serverAddr + "/rule/preview", data).then((response) => {
        this.applicatorResult = response.data.passwordsPreview;
        console.log(this.rulesList)
        this.filterPreviewPasswords();
      }).catch((error) => {
        this.previewPasswords.string = error.message;
      }).finally(() => {
        this.previewPasswords.loading = false; //the loading is done
      })
    },
    filterPreviewPasswords() {
      this.previewPasswordsList = [];
      this.applicatorResult.forEach(element => {
        if (element.retCode >= 0) {
          this.previewPasswordsList.push(element.finalPassword);
        }
      });
      this.previewPasswords.string = this.previewPasswordsList.join("\n");
      console.log(this.previewPasswords.string)
    },
    showInsertPopup(insertData) {
      //this.functionsInsertPopup = Object.assign({}, this.functionInsertPopup, insertData) //change the popup data
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
      this.axios.get(this.$serverAddr + '/rule/' + this.$route.params.id + '/download').then((response) => {
        this.rulesList = response.data.split("\n")
        this.backupRulesList = this.rulesList.slice()
        this.generatePreview("p@ssW0rd"); //generate the initial mangled passwords
      });
      this.axios.get(this.$serverAddr + '/rule/' + this.$route.params.id).then((response) => {
        this.ruleFileInfo = response.data;
      });
    },
    /**
     * Function which checks if the rules has been changed compared to beginning
     */
    rulesChanged() {
      console.log("Baqckup:", this.backupRulesList.toString())
      console.log("CUrrent:", this.rulesList.toString())

      return this.backupRulesList.toString() !== this.rulesList.toString(); 
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
  created() {
    if (this.$route.params.id) { //when there is id parameter in route url
      this.editingFile = true;
      this.loadRuleFile();
    }
  },
  beforeMount() {
    window.addEventListener('beforeunload', this.navigateFromPage);
  },
  beforeDestroy() {
    window.removeEventListener('beforeunload', this.navigateFromPage);
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