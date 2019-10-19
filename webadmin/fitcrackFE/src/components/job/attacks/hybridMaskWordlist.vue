<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-row>
      <v-col
        cols="6"
        class="border"
      >
        <v-card-title class="pb-0">
          <h2>Type mask<span class="required primary--text"> *</span></h2>
        </v-card-title>
      </v-col>
      <v-col cols="6">
        <v-card-title class="pb-0">
          <h2>Select dictionary<span class="required primary--text"> *</span></h2>
        </v-card-title>
      </v-col>

      <v-col
        cols="6"
        class="border"
      >
        <v-expansion-panels class="elevation-0 pt-2">
          <mask-single
            v-model="mask"
            :open-forever="true"
            @input="checkValid"
          />
        </v-expansion-panels>
      </v-col>

      <v-col cols="6">
        <dict-selector
          v-model="dictionaries"
          @input="checkValid"
        />
      </v-col>

      <v-col
        cols="6"
        class="border"
      >
        <v-card-title class="pb-0">
          <h2>Type rule</h2>
        </v-card-title>
      </v-col>
      <v-col cols="6">
        <v-card-title class="pb-0">
          <h2>Type rule</h2>
        </v-card-title>
      </v-col>

      <v-col
        cols="6"
        class="border px-2"
      >
        <v-text-field
          v-model="ruleLeft"
          outlined
          single-line
          placeholder="Rule left"
          @input="checkValid"
        />
      </v-col>
      <v-col
        cols="6"
        class="px-2"
      >
        <v-text-field
          v-model="ruleRight"
          outlined
          single-line
          placeholder="Rule right"
          @input="checkValid"
        />
      </v-col>
    </v-row>
  </div>
</template>

<script>
  import dictSelector from '@/components/selector/dictionarySelector'
  import maskSingle from '@/components/mask/maskSingle'
  export default {
    name: "HybridWordlistMask",
    components: {
      'mask-single': maskSingle,
      'dict-selector': dictSelector
    },
    props: {
      value: {
        type: [Boolean, Object],
        default: null
      },
    },
    data: function () {
      return {
        attackId: 7,
        attackName: 'Hybrid mask+wordlist',
        dictionaries: [],
        mask: '',
        ruleLeft: '',
        ruleRight: '',
        maskRules: [
          v => /^(\?[ludhHsab]|[ -~])+$/.test(v) || 'Not valid mask'
        ]
      }
    },
    watch:{
      value: function(){

      }
    },
    methods: {
      dictSelected: function (id, dictNubmer) {
        this.selectedDict2Id = id
        this.checkValid();
      },
      checkValid: function () {
        if (this.mask !== '' && this.dictionaries.length > 0) {
          this.$emit('input', {
            'attack_mode': this.attackId,
            'attack_name': this.attackName,
            'rule_left': this.ruleLeft,
            'rule_right': this.ruleRight,
            'right_dictionaries': this.dictionaries,
            'mask': this.mask
          })
          return true
        }
        return false
      },
      loadDictionaries: function () {
        this.dictsLoading = true
        this.axios.get(this.$serverAddr + '/dictionary').then((response) => {
          this.dictionaries = response.data
          this.dictsLoading = false
        })
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

  .maskBtn {
    min-width: 0px;
    max-width: 40px;
    text-transform: none;
  }

  .maskBtns {
    text-align: center;
  }

</style>

<style>
  .selected {
    background: rgba(37, 157, 173, 0.85) !important;
    color: white;
  }

  .selected a {
    color: white;
  }

  .clickable {
    cursor: pointer;
  }
</style>
