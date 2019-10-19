<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-card-title class="pb-0">
      <h2>Type masks<span class="required primary--text"> *</span></h2>
    </v-card-title>
    <v-expansion-panels
      v-model="editedMaskIdx"
      popout
      :expand="false"
    >
      <v-row
        v-for="(mask, i) in masks"
        align="center"
        class="width100 fill-height"
      >
        <v-col>
          <mask-single
            v-model="masks[i]"
            :custom-charsets="charsets"
            @input="checkValid"
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
    </v-expansion-panels>
    <div>
      <v-btn
        class="mx-auto d-block"
        color="success"
        icon
        small
        @click="addMask"
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
          <h2>Select charsets (max. 4)</h2>
        </v-card-title>
        <charset-selector
          v-model="charsets"
          @input="checkValid"
        />
      </v-col>
      <v-col cols="6">
        <v-card-title class="pb-0">
          <h2>Markov file</h2>
        </v-card-title>
        <markov-selector
          v-model="markov"
          @input="checkValid"
        />

        <v-row>
          <v-col
            cols="6"
            class="pl-2"
          >
            <v-radio-group
              v-model="markovSubmode"
              column
              :disabled="markov === null"
              @change="checkValid"
            >
              <v-radio
                label="2D Markov"
                color="primary"
                :value="1"
                :disabled="markov === null"
              />
              <v-radio
                label="3D Markov"
                color="primary"
                :value="2"
                :disabled="markov === null"
              />
            </v-radio-group>
          </v-col>
          <v-col
            cols="6"
            class="pr-2"
          >
            <v-text-field
              v-model="markovTreshold"
              type="tel"
              :disabled="markov === null"
              label="Markov treshold"
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
  export default {
    name: "FcMask",
    components: {
      CharsetSelector,
      'mask-single': maskSingle,
      'charset-selector': CharsetSelector,
      'markov-selector': markovSelector,
      'maskFile-selector': maskFileSelector
    },
    props: {
      value: {
        type: [Boolean, Object],
        default: null
      },
    },
    data: function () {
      return {
        loadMasksDialog: false,
        markovSubmode: 0,
        attackId: 3,
        attackName: 'mask',
        markovTreshold: '',
        maskFile: null,
        charsets: null,
        markov: null,
        editedMaskIdx: 0,
        masks: [''],
      }
    },
    watch:{
      value: function(){

      }
    },
    methods: {
      loadMasksFromFile: function () {
        this.axios.get(this.$serverAddr + '/masks/' + this.maskFile.id).then((response) => {
          var masksToDisplay = response.data.data.split("\n")
          if (this.masks[0] === '') {
            this.masks[0] = masksToDisplay[0]
          } else {
            this.masks.push(masksToDisplay[0])
          }
          for (let i = 1; i < masksToDisplay.length; i++) {
            if (masksToDisplay[i] !== '') {
              this.masks.push(masksToDisplay[i])
            }
          }
          this.maskFile = null
          this.loadMasksDialog = false
          this.checkValid()
        })
      },
      checkValid: function () {
        if (this.masks[0] !== '') {
          this.$emit('input', {
            'attack_mode': this.attackId,
            'attack_name': this.attackName,
            'masks': this.masks,
            'attack_submode': this.markovSubmode,
            'markov_treshold': parseInt(this.markovTreshold),
            'markov': this.markov,
            'charset': this.charsets
          })
          return true
        }
        return false
      },
      addMask: function () {
        this.masks.push('')
        this.checkValid()
        this.$nextTick(() => {
          this.editedMaskIdx = this.masks.length - 1
        })
      },
      deleteMask: function (i) {
        if (this.masks.length <= 1 )
          return
        this.masks.splice(i, 1)
        this.checkValid()
      },
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
