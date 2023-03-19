<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container>
    <v-row justify="center">
      <mainEditWindow v-bind:rulesList="rulesList" v-on:rules-updated="updateRules"></mainEditWindow>
      <liveKeyspacePreview v-bind:passwordsList="passwordsList" v-bind:previewPasswordsString="previewPasswordsString"
        v-on:generate-preview="generatePreview">
      </liveKeyspacePreview>
    </v-row>
  </v-container>
</template>

  
<script>
import mainEditWindow from "@/components/rule/mainEditWindow/mainEditWindow.vue";
import liveKeyspacePreview from "@/components/rule/livePreview/liveKeyspacePreview.vue"

export default {

  data() {
    return {
      rulesList: [""],
      passwordsList: [""],
      applicatorResult: "",
      previewPasswords: [],
      previewPasswordsString: "",
    };
  },
  methods: {
    updateRules(updatedRulesList) {
      this.rulesList = updatedRulesList;
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
  },
  components: { mainEditWindow, liveKeyspacePreview }
};
</script>


<style></style>