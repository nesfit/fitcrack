<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-card-title class="pb-0">
      <span>Type masks<span class="required primary--text"> *</span></span>
    </v-card-title>
    <v-row
      v-for="(mask, i) in masks"
      :key="i"
      align="center"
      class="width100 fill-height"
    >
      <v-col>
        <mask-single
          :value="masks[i]"
          :custom-charsets="charset"
          @input="e => updateMask({index: i, val: e})"
        />
      </v-col>
      <v-btn
        text
        color="error"
        icon
        small
        @click="deleteMask(i)"
      >
        <v-icon>close</v-icon>
      </v-btn>
    </v-row>
    <div>
      <v-btn
        class="mx-auto d-block"
        color="success"
        icon
        small
        @click="addMask()"
      >
        <v-icon>add</v-icon>
      </v-btn>
    </div>

    <v-btn
      color="primary"
      outlined
      text
      @click="loadMasksDialog = true"
    >
      Load masks
    </v-btn>

    <v-row>
      <v-col
        cols="6"
        class="border"
      >
        <v-card-title class="pb-0">
          <span>Select charsets (max. 4)</span>
        </v-card-title>
        <charset-selector
          v-model="charset"
        />
      </v-col>
      <v-col cols="6">
        <v-card-title class="pb-0">
          <span>Markov file</span>
        </v-card-title>
        <markov-selector
          v-model="markov"
          :markov-submode="submode"
          @input="checkValid"
        />

        <v-row>
          <v-col
            cols="6"
            class="pl-2"
          >
            <v-radio-group
              v-model="submode"
              column
              @change="checkValid"
            >
              <v-radio
                label="Markov disabled"
                color="primary"
                :value="0"
              />
              <v-radio
                label="2D Markov"
                color="primary"
                :value="1"
              />
              <v-radio
                label="3D Markov"
                color="primary"
                :value="2"
              />
            </v-radio-group>
          </v-col>
          <v-col
            cols="6"
            class="pr-2"
          >
            <v-text-field
              v-model="markovThresh"
              type="number"
              :disabled="submode === 0"
              label="Markov threshold"
              single-line
              mask="########"
              @input="checkValid"
            />
          </v-col>
        </v-row>
      </v-col>
    </v-row>



    <v-dialog
      v-model="loadMasksDialog"
      max-width="400"
    >
      <v-card>
        <maskFile-selector
          v-model="maskFile"
          @input="checkValid"
        />
        <v-btn
          outlined
          text
          class="width96"
          color="primary"
          :disabled="maskFile === null"
          @click="loadMasksFromFile"
        >
          Load
        </v-btn>
      </v-card>
    </v-dialog>
  </div>
</template>

<script>
  import maskSingle from '@/components/mask/maskSingle'
  import CharsetSelector from '@/components/selector/charsetSelector'
  import markovSelector from '@/components/selector/markovSelector'
  import maskFileSelector from '@/components/selector/loadMaskFileSelector'

  import {mapMutations} from 'vuex'
  import {mapTwoWayState} from 'spyfu-vuex-helpers'
  import {twoWayMap} from '@/store'

  export default {
    name: "FcMask",
    components: {
      CharsetSelector,
      'mask-single': maskSingle,
      'charset-selector': CharsetSelector,
      'markov-selector': markovSelector,
      'maskFile-selector': maskFileSelector
    },
    data: function () {
      return {
        loadMasksDialog: false,
        maskFile: null
      }
    },
    computed: {
      ...mapTwoWayState('jobForm', twoWayMap(['masks', 'submode', 'markovThresh', 'markov', 'charset']))
    },
    methods: {
      ...mapMutations('jobForm', ['addMask', 'deleteMask', 'updateMask', 'mergeMasks']),
      loadMasksFromFile: function () {
        this.axios.get(this.$serverAddr + '/masks/' + this.maskFile.id).then((response) => {
          var res = response.data.data.split("\n")
          this.mergeMasks(res)
          this.maskFile = null
          this.loadMasksDialog = false
          this.checkValid()
        })
      },
      checkValid: function () {
        if (this.masks[0] !== '') {
          /*
          this.$emit('input', {
            'attack_mode': this.attackId,
            'attack_name': this.attackName,
            'masks': this.masks,
            'attack_submode': this.markovSubmode,
            'markov_treshold': this.markovSubmode == 0 ? null : parseInt(this.markovTreshold),
            'markov': this.markovSubmode == 0 ? null : this.markov[0],
            'charset': this.charsets
          })
          */
          return true
        }
        return false
      }
    }
  }
</script>

<style scoped>
  .masksContainer {
    overflow: auto;
    max-height: 500px;
  }
  .required {
    font-weight: bold;
    color: #000;
  }

  .maskBtnAction {
    margin-top: 15px;
  }

  .maskBtn {
    width: 40px;
    min-width: 0px;
    text-transform: none;
  }

  .border {
    border-right: 2px solid rgba(0,0,0,.12)
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

  .width96 {
    width: 96%;
  }
</style>
