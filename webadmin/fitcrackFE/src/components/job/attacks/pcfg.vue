<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-card-title>
      <span>Select PCFG grammar<span class="required primary--text"> *</span></span>
    </v-card-title>
    <pcfg-selector
      v-model="pcfg"
      @input="checkValidInit"
    />
    <v-card-title>
      <span>Edit keyspace limit</span>
    </v-card-title>
    <v-text-field
      v-model.number="keyspaceLimit"
      outlined
      single-line
      :value="keyspaceLimit"
      required
      type="number"
      suffix="passwords"
      :max="pcfg.keyspace"
      @input="checkValidEdit"
    />
    <v-divider />
    <v-card-title>
      <span>Select rule file</span>
    </v-card-title>
    <rules-selector 
      v-model="rules"
      @input="checkValidInit"
    />
  </div>
</template>

<script>
  import pcfgSelector from '@/components/selector/pcfgSelector'
  import ruleSelector from '@/components/selector/rulesSelector'

  import {mapTwoWayState} from 'spyfu-vuex-helpers'
  import {twoWayMap} from '@/store'

  export default {
    name: "Pcfg",
    components: {
      'pcfg-selector': pcfgSelector,
      'rules-selector': ruleSelector
    },
    data: function () {
      return {
        attackId: 9,
        attackName: 'pcfg',
      }
    },
    computed: mapTwoWayState('jobForm', twoWayMap(['rules', 'pcfg', 'keyspaceLimit'])),
    methods: {
      checkValidInit: function () {
        if(this.keyspaceLimit !== this.pcfg[0].keyspace)
          this.keyspaceLimit = this.pcfg[0].keyspace
        if(this.keyspaceLimit > this.pcfg[0].keyspace)
          this.keyspaceLimit = this.pcfg[0].keyspace
          /*  if (this.pcfg[0]_grammmar.length > 0) {
              this.$emit('input', {
                'attack_mode': this.attackId,
                'attack_name': this.attackName,
                'rules': this.rules,
                'pcfg_grammar': this.pcfg[0],
                'keyspace_limit': this.keyspaceLimit
              })
              return true
          //  }
          //  return false
          */
        },
        checkValidEdit: function () {
          if(this.keyspaceLimit > this.pcfg[0].keyspace)
            this.keyspaceLimit = this.pcfg[0].keyspace

        /*  if (this.pcfg_grammmar.length > 0) {
            this.$emit('input', {
              'attack_mode': this.attackId,
              'attack_name': this.attackName,
              'rules': this.rules,
              'pcfg_grammar': this.pcfg,
              'keyspace_limit': this.keyspaceLimit
            })
            return true
        //  }
        //  return false
        */
      }
    }
  }
</script>

<style scoped>
  .required {
    font-weight: bold;
  }
</style>
