<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container>
    <v-row justify="center">
      <mainEditWindow></mainEditWindow>
      <liveKeyspacePreview></liveKeyspacePreview>
    </v-row>
  </v-container>
</template>
  

  
<script>
import mainEditWindow from "@/components/rule/mainEditWindow/mainEditWindow.vue";
import liveKeyspacePreview from "@/components/rule/livePreview/liveKeyspacePreview.vue"

export default {

  data() {
    return {
    };
  },
  methods: {
  },
  components: { mainEditWindow, liveKeyspacePreview }
};
</script>


<style>

</style>