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
      <mainEditWindow v-bind:rulesList="rulesList" v-on:rules-updated="updateRules"
        v-on:show-insert-popup="showInsertPopup" v-on:show-all-functions-popup="showAllFunctionsPopup"></mainEditWindow>
      <liveKeyspacePreview v-bind:passwordsList="passwordsList" v-bind:previewPasswordsString="previewPasswordsString"
        v-on:generate-preview="generatePreview">
      </liveKeyspacePreview>
    </v-row>
  </v-container>
</template>

  
<script>
import Vue from 'vue';
import mainEditWindow from "@/components/rule/mainEditWindow/mainEditWindow.vue";
import liveKeyspacePreview from "@/components/rule/livePreview/liveKeyspacePreview.vue"
import functionInsertPopup from "@/components/rule/mainEditWindow/popups/functionInsertPopup.vue"
import allFunctionsPopup from '@/components/rule/mainEditWindow/popups/allFunctionsPopup.vue';


export default {

  data() {
    return {
      rulesList: [""],
      passwordsList: [""],
      applicatorResult: "",
      previewPasswords: [],
      previewPasswordsString: "",
      functionsInsertPopup: {
        visible: false,
        functionIndex: 0,
        ruleIndex: 0
      },
      allFunctionsPopup: {
        visible: false,
        onlyShow: true
      }
    };
  },
  methods: {
    updateRules(updatedRulesList) {
      this.rulesList = updatedRulesList;
    },
    updateRule(changedRule) {
      console.log(this.functionsInsertPopup)
      //update the rule, where rule function was inserted
      Vue.set(this.rulesList, this.functionsInsertPopup.ruleIndex, this.rulesList[this.functionsInsertPopup.ruleIndex].trimEnd().concat(changedRule));

    },
    generatePreview(updatedPasswordsList) {
      this.passwordsList = updatedPasswordsList;
      const data = {
        rulesList: this.rulesList,
        passwordsList: this.passwordsList
      };
      this.axios.post(this.$serverAddr + "/rule/preview", data).then((response) => {
        this.applicatorResult = response.data.passwordsPreview;
        this.filterPreviewPasswords();
      }).catch((error) => {
        this.previewPasswords = error.message;
      });
    },
    filterPreviewPasswords() {
      this.previewPasswords = [];
      this.applicatorResult.forEach(element => {
        if (element.retCode >= 0) {
          this.previewPasswords.push(element.finalPassword);
        }
      });
      this.previewPasswordsString = this.previewPasswords.join("\n");
    },
    showInsertPopup(insertData) {
      this.functionsInsertPopup = insertData;
    },
    hideInsertPopup(visibility) {
      this.functionsInsertPopup.visible = visibility;
    },
    showAllFunctionsPopup(popupData) {
      this.allFunctionsPopup = popupData;
    },
    hideAllFunctionsPopup(visibility) {
      this.allFunctionsPopup.visible = visibility;
    }
  },
  components: { mainEditWindow, liveKeyspacePreview, functionInsertPopup, allFunctionsPopup }
};
</script>


<style></style>