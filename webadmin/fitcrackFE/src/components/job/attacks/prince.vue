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
    <v-card-title>
      <span>Minimal length of passwords (1 - 16)</span>
    </v-card-title>
    <v-card-text>
      <v-text-field
        v-model.number="minPasswordLen"
        text
        single-line
        :value="minPasswordLen"
        required
        type="number"
        min="1"
        max="16"
        @input="checkValid"
      />
    </v-card-text>

    <v-divider />
    <v-card-title>
      <span>Maximal length of passwords (1 - 16)</span>
    </v-card-title>
    <v-card-text>
      <v-text-field
        v-model.number="maxPasswordLen"
        text
        single-line
        :value="maxPasswordLen"
        required
        type="number"
        min="1"
        max="16"
        @input="checkValid"
      />
    </v-card-text>

        <v-divider />
    <v-card-title>
      <span>Minimal number of elements per chain (1 - 16)</span>
    </v-card-title>
    <v-card-text>
      <v-text-field
        v-model.number="minElemInChain"
        text
        single-line
        :value="minElemInChain"
        required
        type="number"
        min="1"
        max="16"
        @input="checkValid"
      />
    </v-card-text>

    <v-divider />
    <v-card-title>
      <span>Maximal number of elements per chain (1 - 16)</span>
    </v-card-title>
    <v-card-text>
      <v-text-field
        v-model.number="maxElemInChain"
        text
        single-line
        :value="maxElemInChain"
        required
        type="number"
        min="1"
        max="16"
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
        if (this.minPasswordLen <= 0) {
            this.$error('Minimal length of passwords must be greater than 0.')
            return false;
        }
        if (this.maxPasswordLen <= 0) {
            this.$error('Maximal length of passwords must be greater than 0.')
            return false;
        }
        if (this.maxPasswordLen > 32) {
            this.$error('Maximal length of passwords must be smaller or equal to 32.')
            return false;
        }
        if (this.minElemInChain <= 0) {
            this.$error('Minimal number of elements per chain must be greater than 0.')
            return false;
        }
        if (this.minPasswordLen > this.maxPasswordLen) {
            this.$error('Minimal length of passwords must be greater than maximal length of passwords.')
            return false;
        }
        if (this.maxElemInChain > 0 && (this.minElemInChain > this.maxElemInChain)) {
            this.$error('Minimal number of elements per chain must be smaller or equal than maximal number of elements per chain.')
            return false;
        }
        if (this.minPasswordLen <= 0) {
            this.$error('Minimal length of passwords must be greater than 0.')
            return false;
        }
        if (this.maxElemInChain > this.maxPasswordLen) {
            this.$error('Maximal number of elements per chain must be smaller or equal than maximal length of passwords.')
            return false;
        }

        return true
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

