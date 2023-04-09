<template>
  <v-container class="max600">
    <fc-tile
      title="Mask generator"
      class="ma-2"
      :icon="$route.meta.icon"
    >
      <v-row style="margin-bottom: -10mm;">
        <v-col cols="4">
          <v-card-title class="pb-0 mb-2">
            <span>Patterns</span>
          </v-card-title>
        </v-col>
        <v-col cols="8" align="right">
          <v-btn class="text-lowercase"
            :disabled="charset1 == ''"
            color="primary"
            outlined
            @click="updatePattern('?1')"
          >
            ?1
          </v-btn>
          <v-btn class="text-lowercase"
          :disabled="charset2 == ''"
            color="primary"
            outlined
            @click="updatePattern('?2')"
          >
            ?2
          </v-btn>
          <v-btn class="text-lowercase"
          :disabled="charset3 == ''"
            color="primary"
            outlined
            @click="updatePattern('?3')"
          >
            ?3
          </v-btn>
          <v-btn class="text-lowercase"
          :disabled="charset4 == ''"
            color="primary"
            outlined
            @click="updatePattern('?4')"
          >
            ?4
          </v-btn>
        </v-col>
      </v-row>
      <v-row>
        <v-col align="right">
          <div class="bottom-space">
            <v-btn class="text-lowercase"
              color="primary"
              outlined
              @click="updatePattern('?l')"
            >
              ?l
            </v-btn>
            <v-btn class="text-lowercase"
              color="primary"
              outlined
              @click="updatePattern('?u')"
            >
              ?u
            </v-btn>
            <v-btn class="text-lowercase"
              color="primary"
              outlined
              @click="updatePattern('?d')"
            >
              ?d
            </v-btn>
            <v-btn class="text-lowercase"
              color="primary"
              outlined
              @click="updatePattern('?s')"
            >
              ?s
            </v-btn>
            <v-btn class="text-lowercase"
              color="primary"
              outlined
              @click="updatePattern('?a')"
            >
              ?a
            </v-btn>
          </div>
        </v-col>
      </v-row>
      <v-text-field
        label="Enter mask pattern"
        filled
        outlined
        dense
        single-line
        v-model="pattern"
      />
      <v-row>
        <v-col align="left">
          <v-btn
            color="primary"
            outlined
            @click="includePattern()"
          >
            Include pattern
            <v-icon right>
              mdi-plus-circle
            </v-icon>
          </v-btn>
        </v-col>
        <v-col align="right">
          <v-btn
            color="primary"
            outlined
            @click="excludePattern()"
          >
            Exclude pattern
            <v-icon right>
              mdi-minus-circle
            </v-icon>
          </v-btn>
        </v-col>
      </v-row>
      <v-row>
        <v-col align="left" cols="5">
          <ul style="list-style-type:disc;">
            <li v-for="incPattern in incPatterns" :key="incPattern.id">
              {{ incPattern.text }}
              <button @click="removeIncPattern(incPattern)">
                <v-icon right>
                  mdi-close-outline
                </v-icon>
              </button>
            </li>
          </ul>
        </v-col>
        <v-col cols="2"></v-col>
        <v-col align="left" cols="5">
          <ul style="list-style-type:disc;">
            <li v-for="excPattern in excPatterns" :key="excPattern.id">
              {{ excPattern.text }}
              <button @click="removeExcPattern(excPattern)">
                <v-icon right>
                  mdi-close-outline
                </v-icon>
              </button>
            </li>
          </ul>
        </v-col>
      </v-row>
      <v-card-title class="pb-0 mb-2">
        <span>Custom character sets</span>
      </v-card-title>
      <v-row>
        <v-col cols="6" align="left">
          <v-text-field
            label="Character set 1"
            filled
            outlined
            dense
            single-line
            v-model="charset1"
          />
        </v-col>
        <v-col cols="6" align="left">
          <v-text-field
            label="Character set 2"
            filled
            outlined
            dense
            single-line
            v-model="charset2"
          />
        </v-col>
      </v-row>
      <v-row>
        <v-col cols="6" align="left">
          <v-text-field
            label="Character set 3"
            filled
            outlined
            dense
            single-line
            v-model="charset3"
          />
        </v-col>
        <v-col cols="6" align="left">
          <v-text-field
            label="Character set 4"
            filled
            outlined
            dense
            single-line
            v-model="charset4"
          />
        </v-col>
      </v-row>
      <v-card-title class="pb-0 mb-2">
        <span>Criteria</span>
      </v-card-title>
      <v-row max-height="50px">
        <v-col cols="2">
        </v-col>
        <v-col cols="2" align="right">
          <b>Min</b>
        </v-col>
        <v-col cols="2" align="right">
          <b>Max</b>
        </v-col>
        <v-col cols="2">
        </v-col>
        <v-col cols="2" align="right">
          <b>Min</b>
        </v-col>
        <v-col cols="2" align="right">
          <b>Max</b>
        </v-col>
      </v-row>
      <v-row>
        <v-col cols="2">
          <b>Lowercase</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="minLower" type="number" min="0" max="99" size="4" @input="checkMinInc()">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxLower" type="number" min="0" max="99" size="4" @input="checkMaxDec()">
        </v-col>
        <v-col cols="2">
          <b>Digits</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="minDigits" type="number" min="0" max="99" size="4" @input="checkMinInc()">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxDigits" type="number" min="0" max="99" size="4" @input="checkMaxDec()">
        </v-col>
      </v-row>
      <v-row>
        <v-col cols="2">
          <b>Uppercase</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="minUpper" type="number" min="0" max="99" size="4" @input="checkMinInc()">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxUpper" type="number" min="0" max="99" size="4" @input="checkMaxDec()">
        </v-col>
        <v-col cols="2">
          <b>Special</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="minSpecial" type="number" min="0" max="99" size="4" @input="checkMinInc()">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxSpecial" type="number" min="0" max="99" size="4" @input="checkMaxDec()">
        </v-col>
      </v-row>
      <v-row class="bottom-space">
        <v-col cols="2">
          <b>Length</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="minLength" type="number" min="0" max="99" size="4" @input="checkMinInc()">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxLength" type="number" min="0" max="99" size="4" @input="checkMaxDec()">
        </v-col>
        <v-col cols="6"/>
      </v-row>
      <v-card-title class="pb-0 mb-2 bottom-space">
        <span>Cracking task options</span>
      </v-card-title>
      <v-row class="top-space">
        <v-col cols="2">
          <b>Time:</b>
        </v-col>
        <v-col cols="1">
          <input v-model="timeHours" type="number" min="0" size="4">
        </v-col>
        <v-col cols="1">
          <b>h</b>
        </v-col>
        <v-col cols="1">
          <input v-model="timeMins" type="number" min="0" size="4" max="59">
        </v-col>
        <v-col cols="1">
          <b>m</b>
        </v-col>
        <v-col cols="2" align="right">
          <b>Speed:</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="speed" type="number" min="0" size="8" step="10000">
        </v-col>
        <v-col cols="2" align="left">
          <b>keys/s</b>
        </v-col>
      </v-row>
      <v-card-title class="pb-0 mb-2">
        <span>Wordlists</span>
      </v-card-title>
      <v-data-table
        :headers="headers"
        :items="dictionaries.items"
        :loading="loading"
        :footer-props="{itemsPerPageOptions: [5,10,25], itemsPerPageText: 'Dictionaries per page'}"
      >
        <template v-slot:item.name="{ item }">
          <router-link :to="`dictionaries/${item.id}`">
            {{ item.name }}
          </router-link>
        </template>
        <template v-slot:item.time="{ item }">
          {{ $moment.utc(item.time).local().format('DD.MM.YYYY HH:mm') }}
        </template>
        <template v-slot:item.keyspace="{ item }">
          {{ fmt(item.keyspace) }}
        </template>
        <template v-slot:item.analyze="{ item }">
          <v-tooltip top>
            <template v-slot:activator="{ on }">
                <v-btn icon
                @click="addDictionary(item.name)">
                  <v-icon>{{ selectedDictionaries.includes(item.name) ? 'mdi-minus-circle' : 'mdi-plus-circle' }}</v-icon>
                </v-btn>
            </template>
            <span>Analyze</span>
          </v-tooltip>
        </template>
      </v-data-table>
      <v-row class="top-space">
        <v-col cols="6">
          <b>Minimum number of occurrences:</b>
        </v-col>
        <v-col cols="6" align="left">
          <input v-model="minOcc" type="number" min="1" size="4">
        </v-col>
      </v-row>
      <v-row>
        <v-col cols="3" align-self="center">
          <b>Sorting mode:</b>
        </v-col>
        <v-col cols="4">
          <v-select
            v-model="sortingMode"
            :items="['Optimal', 'Occurrence', 'Complexity']"
          ></v-select>
        </v-col>
      </v-row>
      <v-row class="bottom-space">
        <v-col cols="6" align="left">
          <v-text-field
            label="Mask file name"
            filled
            outlined
            dense
            single-line
            v-model="filename"
          />
        </v-col>
        <v-col cols="6" align="right">
          <v-btn
              color="primary"
              outlined
              @click="showOrdering()"
            >
            {{ orderingVisible ? "Close charset priorities" : "Change charset priorities" }}
          </v-btn>
        </v-col>
      </v-row>
      <v-data-table
        class="bottom-space"
        v-if="orderingVisible"
        :headers="charsetOrderHeaders"
        :items="charsetOrderList"
        :loading=false
        :sort-by="['order']"
        :hide-default-footer="true"
      >
        <template v-slot:item.order="{ item }">
          {{ item.order }}
        </template>
        <template v-slot:item.name="{ item }">
          {{ item.name }}
        </template>
        <template v-slot:item.placeholder="{ item }">
          {{ item.placeholder }}
        </template>
        <template v-slot:item.move="{ item }">
          <v-tooltip top>
            <template v-slot:activator="{ on }">
                <v-btn icon
                @click="moveCharsetUp(item.name)">
                  <v-icon>mdi-arrow-up</v-icon>
                </v-btn>
                <v-btn icon
                @click="moveCharsetDown(item.name)">
                  <v-icon>mdi-arrow-down</v-icon>
                </v-btn>
            </template>
            <span>Analyze</span>
          </v-tooltip>
        </template>
      </v-data-table>
      <v-row>
        <v-col align="right">      
          <v-btn
            color="primary"
            outlined
            @click="generateMasks()"
          >
            {{ showItemState }}
          </v-btn>
        </v-col>
      </v-row>
    </fc-tile>
  </v-container>
</template>

<script>
  let incId = 0
  let excId = 0

  import fmt from '@/assets/scripts/numberFormat'
  import tile from '@/components/tile/fc_tile.vue'
  export default {
    data: function () {
      return {
        orderingVisible: false,
        loading: false,
        awaitingResponse: false,
        pattern: '',
        minLower: 0,
        minUpper: 0,
        maxLower: 0,
        maxUpper: 0,
        minDigits: 0,
        maxDigits: 0,
        minSpecial: 0,
        maxSpecial: 0,
        maxLength: 0,
        minLength: 0,
        timeHours: 0,
        timeMins: 0,
        speed: 100000,
        minOcc: 1,
        sortingMode: 'Optimal',
        charset1: '',
        charset2: '',
        charset3: '',
        charset4: '',
        incPatterns: [],
        excPatterns: [],
        headers: [
          {
            text: 'Name',
            align: 'start',
            value: 'name'
          },
          {text: 'Keyspace', value: 'keyspace', align: 'end'},
          {text: 'Time', value: 'time', align: 'end'},
          {text: 'Analyze', value: 'analyze', align: 'end', sortable: false}
        ],
        charsetOrderHeaders: [
          {
            text: 'Order',
            align: 'start',
            value: 'order',
            sortable: false
          },
          {text: 'Name', value: 'name', align: 'end', sortable: false},
          {text: 'Placeholder', value: 'placeholder', align: 'end', sortable: false},
          {text: 'Move', value: 'move', align: 'end', sortable: false}
        ],
        charsetOrderList: [ {order: 1, name: 'Custom character set 1', placeholder: '?1'},
                            {order: 2, name: 'Custom character set 2', placeholder: '?2'},
                            {order: 3, name: 'Custom character set 3', placeholder: '?3'},
                            {order: 4, name: 'Custom character set 4', placeholder: '?4'},
                            {order: 5, name: 'Lowercase letters', placeholder: '?l'},
                            {order: 6, name: 'Uppercase letters', placeholder: '?u'},
                            {order: 7, name: 'Digits', placeholder: '?d'},
                            {order: 8, name: 'Special characters', placeholder: '?s'},
                            {order: 9, name: 'Lowercase hexadecimal', placeholder: '?h'},
                            {order: 10, name: 'Uppercase hexadecimal', placeholder: '?H'},],
        dictionaries: [],
        selectedDictionaries: [],
        filename: ''
      }
    },
    mounted: function () {
      this.loadDictionaries()
    },
    name: "MaskGenerator",
    components: {
      'fc-tile': tile,
    },
    computed: {
      showItemState () {
        if (!this.awaitingResponse) return "Generate masks"
        else return "Generating..."
      }
    },
    methods: {
      fmt,
      updatePattern: function (text) {
        this.pattern += text
      },
      showOrdering: function () {
        this.orderingVisible = !this.orderingVisible
      },
      includePattern: function () {
        this.incPatterns.push({ id: incId++, text: this.pattern })
        this.pattern = ''
      },
      excludePattern: function () {
        this.excPatterns.push({ id: excId++, text: this.pattern })
        this.pattern = ''
      },
      removeIncPattern: function (incPattern) {
        this.incPatterns = this.incPatterns.filter((b) => b !== incPattern)
      },
      removeExcPattern: function (excPattern) {
        this.excPatterns = this.excPatterns.filter((b) => b !== excPattern)
      },
      moveCharsetUp: function (charsetName) {
        let charset = this.charsetOrderList.find(charset => charset.name == charsetName)
        if (charset.order != 1) {
          let charset2 = this.charsetOrderList.find(charset2 => charset2.order == charset.order - 1)
          charset.order = charset.order - 1
          charset2.order = charset2.order + 1
        }
      },
      moveCharsetDown: function (charsetName) {
        let charset = this.charsetOrderList.find(charset => charset.name == charsetName)
        if (charset.order != 10) {
          let charset2 = this.charsetOrderList.find(charset2 => charset2.order == charset.order + 1)
          charset.order = charset.order + 1
          charset2.order = charset2.order - 1
        }
      },
      addDictionary: function (dictName) {
        if (this.selectedDictionaries.includes(dictName)) {
          this.selectedDictionaries.splice(this.selectedDictionaries.indexOf(dictName), 1)
        }
        else {
          this.selectedDictionaries.push(dictName)
        }
      },
      loadDictionaries: function () {
        this.loading = true;
        this.axios.get(this.$serverAddr + '/dictionary', {}).then((response) => {
          this.dictionaries = response.data;
          this.loading = false
        })
      },
      checkMinInc: function () {
        if (this.minLower > this.maxLower) this.maxLower = this.minLower;
        if (this.minUpper > this.maxUpper) this.maxUpper = this.minUpper;
        if (this.minSpecial > this.maxSpecial) this.maxSpecial = this.minSpecial;
        if (this.minDigits > this.maxDigits) this.maxDigits = this.minDigits;
        if (this.minLength > this.maxLength) this.maxLength = this.minLength;
      },
      checkMaxDec: function () {
        if (this.minLower > this.maxLower) this.minLower = this.maxLower;
        if (this.minUpper > this.maxUpper) this.minUpper = this.maxUpper;
        if (this.minSpecial > this.maxSpecial) this.minSpecial = this.maxSpecial;
        if (this.minDigits > this.maxDigits) this.minDigits = this.maxDigits;
        if (this.minLength > this.maxLength) this.minLength = this.maxLength;
      },
      generateMasks: function () {
        var patexc = []
        this.excPatterns.forEach(function(pattern){
          patexc.push(pattern['text'])
        })
        var patinc = []
        this.incPatterns.forEach(function(pattern){
          patinc.push(pattern['text'])
        })
        this.awaitingResponse = true;
        this.axios.post(this.$serverAddr + '/maskGenerator', {
          minlength: this.minLength,
          maxlength: this.maxLength,
          minlower: this.minLower,
          minupper: this.minUpper,
          maxlower: this.maxLower,
          maxupper: this.maxUpper,
          mindigit: this.minDigits,
          maxdigit: this.maxDigits,
          minspecial: this.minSpecial,
          maxspecial: this.maxSpecial,
          time: this.timeHours * 3600 + this.timeMins * 60,
          speed: this.speed,
          minocc: this.minOcc,
          sortingMode: this.sortingMode,
          charset1: this.charset1,
          charset2: this.charset2,
          charset3: this.charset3,
          charset4: this.charset4,
          patinc: patinc,
          patexc: patexc,
          wordlists: this.selectedDictionaries,
          filename: this.filename,
          charsetOrderList: this.charsetOrderList
        })
        .then(response => {
          console.log(response);
          this.awaitingResponse = false;
        })
        .catch(error => {
          console.log(error);
          this.awaitingResponse = false;
        });
      }
    }
  }
</script>

<style scoped>

  .max600 {
      max-width: 600px;
    }

</style>

<style>

  .bottom-space {
      margin-bottom: 4mm;
    }

  .top-space {
      margin-top: 4mm;
    }

  .align-right {
      text-align: right;
    }

</style>