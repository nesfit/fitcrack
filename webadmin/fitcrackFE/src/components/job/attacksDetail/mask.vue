<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <fc-tile title="Attack details">
    <v-list single-line class="width100">
      <v-list-tile class="px-2 py-1">
        <v-list-tile-action class="pr-3 key">
          Type:
        </v-list-tile-action>
        <v-list-tile-content>
          <v-list-tile-title class="text-xs-right">{{data.attack}}</v-list-tile-title>
        </v-list-tile-content>
      </v-list-tile>
      <template v-if="data.charSet1.id !== null">
        <v-divider></v-divider>
        <v-list-tile class="px-2 py-1">
          <v-list-tile-action class="pr-3 key">
            Charset 1:
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title class="text-xs-right">
              <router-link :to="{name: 'charsetDetail', params: { id: data.charSet1.id}}">{{ data.charSet1.name }}</router-link>
            </v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
      </template>
      <template v-if="data.charSet2.id !== null">
        <v-divider></v-divider>
        <v-list-tile class="px-2 py-1">
          <v-list-tile-action class="pr-3 key">
            charset 2:
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title class="text-xs-right">
              <router-link :to="{name: 'charsetDetail', params: { id: data.charSet2.id}}">{{ data.charSet2.name }}</router-link>
            </v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
      </template>
      <template v-if="data.charSet3.id !== null">
        <v-divider></v-divider>
        <v-list-tile class="px-2 py-1">
          <v-list-tile-action class="pr-3 key">
            Charset 3:
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title class="text-xs-right">
              <router-link :to="{name: 'charsetDetail', params: { id: data.charSet3.id}}">{{ data.charSet3.name }}</router-link>
            </v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
      </template>
      <template v-if="data.charSet4.id !== null">
        <v-divider></v-divider>
        <v-list-tile class="px-2 py-1">
          <v-list-tile-action class="pr-3 key">
            Charset 4:
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title class="text-xs-right">
              <router-link :to="{name: 'charsetDetail', params: { id: data.charSet4.id}}">{{ data.charSet4.name }}</router-link>
            </v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
      </template>

      <template v-if="data.markov.id !== null">
        <v-divider></v-divider>
        <v-list-tile class="px-2 py-1">
          <v-list-tile-action class="pr-3 key">
            Markov:
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title class="text-xs-right">
              <a target="_blank" :href="$serverAddr + '/markovChains/' + data.markov.id">{{
                data.markov.name }}</a>
            </v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
      </template>

      <v-divider></v-divider>
      <v-data-table :headers="maskHeaders"
                    :items="data.masks"
                    disable-initial-sort
                    hide-actions
      >
        <template slot="items" slot-scope="props">
          <td v-html="parseHTML(props.item.mask)"></td>
          <td class="text-xs-right">
            <v-progress-circular
              size="35"
              :width="1.5"
              :rotate="270"
              color="primary"
              :value="props.item.progress"
            >
              <span class="progressPercentageMask">{{ progressToPercentage(props.item.progress) }}</span>
            </v-progress-circular>
          </td>
        </template>
      </v-data-table>
    </v-list>
  </fc-tile>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "maskDetail",
    props: {
      data: Object,
      customCharsets: {
        type: [Array, null],
        default: null
      }
    },
    components: {
      'fc-tile': tile
    },
    methods: {
      progressToPercentage: function (progress) {
        return progress.toFixed() + '%'
      },
      parseHTML(mask) {
        console.log(mask)
        var nextCharSymbol = false
        var parsed = ''
        for (var i = 0; i < mask.length; i++) {
          if (nextCharSymbol) {
            if (mask[i] === '?') {
              parsed += '?'
              nextCharSymbol = false
              continue
            }
            var symbol = this.represenArray[mask[i]]
            if (symbol === undefined) {
              symbol = {
                represent: mask[i]
              }
            }
            parsed += '<span class="symbol primary--text">' + symbol.represent + '</span>'
            nextCharSymbol = false
            continue
          }
          if (mask[i] === '?') {
            nextCharSymbol = true
          } else {
            parsed += mask[i]
            nextCharSymbol = false
          }
        }
        return parsed
      }
    },
    data: function () {
      return {
        maskHeaders: [
          {
            text: 'Mask',
            align: 'left',
            value: 'mask'
          },
          {text: 'Mask progress', value: 'progress', align: 'right'}
        ],
        represenArray: {
          'l': {
            'chars': 'abcdefghijklmnopqrstuvwxyz',
            'represent': 'a-z'
          },
          'u': {
            'chars': 'ABCDEFGHIJKLMNOPQRSTUVWXYZ',
            'represent': 'A-Z'
          },
          'd': {
            'chars': '0123456789',
            'represent': '0-9'
          },
          'h': {
            'chars': '0123456789abcdef',
            'represent': '0-f'
          },
          'H': {
            'chars': '0123456789ABCDEF',
            'represent': '0-F'
          },
          's': {
            'chars': '«space»!"#$%&()*+,-./:;<=>?@[]^_`{|}~',
            'represent': 'special'
          },
          'a': {
            'chars': '?l?u?d?s',
            'represent': 'a-z,A-Z,0-9,special'
          },
          'b': {
            'chars': '0x00 - 0xff',
            'represent': 'ASCII'
          }
        }
      }
    }
  }
</script>

<style scoped>

</style>
<style >
  .symbol {
    margin-left: 1px;
    margin-right: 1px;
    border: 1px solid currentColor;
    padding: 3px;
    border-radius: 4px;
    font-weight: 500;
  }
</style>
