<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-card-title>
      <span>Select dictionary<span class="required primary--text"> *</span></span>
    </v-card-title>
    <dict-selector
      v-model="leftDicts"
      select-all
      @input="checkValid"
    />

    <v-divider />
    <v-card-title>
      <span>Select rule file</span>
    </v-card-title>
    <rules-selector
      v-model="rules"
      @input="checkValid"
    />
  </div>
</template>

<script>
  import dictSelector from '@/components/selector/dictionarySelector.vue'
  import ruleSelector from '@/components/selector/rulesSelector.vue'

  import {mapTwoWayState} from 'spyfu-vuex-helpers'
  import {twoWayMap} from '@/store'

  export default {
    name: "Dictionary",
    components: {
      'dict-selector': dictSelector,
      'rules-selector': ruleSelector
    },
    computed: mapTwoWayState('jobForm', twoWayMap(['leftDicts', 'rules'])),
    methods: {
      checkValid: function () {
        if (this.leftDicts.length > 0) {
          return true
        }
        return false
      }
    }
  }
</script>

<style scoped>
  .required {
    font-weight: bold;
    color: #000;
  }
</style>
