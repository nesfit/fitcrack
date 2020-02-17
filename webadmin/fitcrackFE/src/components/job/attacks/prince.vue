<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-card-title class="pb-0">
      <span>Select dictionary<span class="required primary--text"> *</span></span>
    </v-card-title>
    <dict-selector
      v-model="leftDicts"
      @input="checkValid"
    />
    <v-divider />

     <v-card-title class="pb-0">
      <span>Select rule file</span>
    </v-card-title>
    <rules-selector 
      v-model="rules"
      @input="checkValid"
    />
    <v-divider />

    <v-checkbox
       v-model="checkDuplicates"
       label="Check password duplicates"
    />
    <v-divider />
    <v-checkbox
       v-model="casePermute"
       label="Case permutation"
    />

    <v-divider />
    <v-card-title class="pb-0">
      <span>Minimal length of password</span>
    </v-card-title>
    <v-card-text>
      <v-text-field
        v-model.number="minPasswordLen"
        text
        single-line
        :value="minPasswordLen"
        required
        type="number"
        @input="checkValid"
      />
    </v-card-text>

    <v-divider />
    <v-card-title class="pb-0">
      <span>Maximal length of password</span>
    </v-card-title>
    <v-card-text>
      <v-text-field
        v-model.number="maxPasswordLen"
        text
        single-line
        :value="maxPasswordLen"
        required
        type="number"
        @input="checkValid"
      />
    </v-card-text>

        <v-divider />
    <v-card-title class="pb-0">
      <span>Minimum number of elements per chain</span>
    </v-card-title>
    <v-card-text>
      <v-text-field
        v-model.number="minElemInChain"
        text
        single-line
        :value="minElemInChain"
        required
        type="number"
        @input="checkValid"
      />
    </v-card-text>

    <v-divider />
    <v-card-title class="pb-0">
      <span>Maximal number of elements per chain</span>
    </v-card-title>
    <v-card-text>
      <v-text-field
        v-model.number="maxElemInChain"
        text
        single-line
        :value="maxElemInChain"
        required
        type="number"
        @input="checkValid"
      />
    </v-card-text>

  </div>
</template>

<script>
  import dictSelector from '@/components/selector/dictionarySelector'
  import ruleSelector from '@/components/selector/rulesSelector'

  import {mapTwoWayState} from 'spyfu-vuex-helpers'
  import {twoWayMap} from '@/store'

  export default {
    name: "Prince",
    components: {
      'dict-selector': dictSelector,
      'rules-selector': ruleSelector
    },
    computed: mapTwoWayState('jobForm', twoWayMap(['leftDicts', 'rules', 'checkDuplicates',
    'casePermute', 'minPasswordLen', 'maxPasswordLen', 'minElemInChain', 'maxElemInChain'])),
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
  .border {
    border-right: 2px solid rgba(0,0,0,.12)
  }

  .required {
    font-weight: bold;
    color: #000;
  }
</style>

