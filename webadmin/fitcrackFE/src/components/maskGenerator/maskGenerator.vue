<template>
  <v-container class="max600">
    <fc-tile
      title="Mask generator"
      class="ma-2"
      :icon="$route.meta.icon"
    >
      <v-row>
        <v-col cols="4">
          <v-card-title class="pb-0 mb-2">
            <span>Patterns</span>
          </v-card-title>
        </v-col>
        <v-col cols="8">
          <div class="align-right">
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
        <v-col align="right">
          <ul style="list-style-type:none;">
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
        <v-col align="right">
          <ul style="list-style-type:none;">
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
          <input v-model="minLower" type="number" size="4">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxLower" type="number" size="4">
        </v-col>
        <v-col cols="2">
          <b>Digits</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="minDigits" type="number" size="4">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxDigits" type="number" size="4">
        </v-col>
      </v-row>
      <v-row>
        <v-col cols="2">
          <b>Uppercase</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="minUpper" type="number" size="4">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxUpper" type="number" size="4">
        </v-col>
        <v-col cols="2">
          <b>Special</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="minSpecial" type="number" size="4">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxSpecial" type="number" size="4">
        </v-col>
      </v-row>
      <v-row class="bottom-space">
        <v-col cols="2">
          <b>Length</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="minLength" type="number" size="4">
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="maxLength" type="number" size="4">
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
          <input v-model="timeHours" type="number" size="4">
        </v-col>
        <v-col cols="1">
          <b>h</b>
        </v-col>
        <v-col cols="1">
          <input v-model="timeMins" type="number" size="4">
        </v-col>
        <v-col cols="1">
          <b>m</b>
        </v-col>
        <v-col cols="2" align="right">
          <b>Speed:</b>
        </v-col>
        <v-col cols="2" align="right">
          <input v-model="speed" type="number" size="8">
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
      <v-row>
        <v-col cols="6">
        <b>Minimum number of occurrences:</b>
        </v-col>
        <v-col cols="6" align="left">
          <input v-model="minOcc" type="number" size="4">
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
        <v-col cols="5"/>
      </v-row>
      <div class="align-right">
          <v-btn
            color="primary"
            outlined
            @click="generateMasks()"
          >
            Generate masks
          </v-btn>
        </div>
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
        loading: false,
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
        speed: 10000,
        minOcc: 0,
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
        dictionaries: [],
        selectedDictionaries: []
      }
    },
    mounted: function () {
      this.loadDictionaries()
    },
    name: "MaskGenerator",
    components: {
      'fc-tile': tile,
    },
    methods: {
      fmt,
      updatePattern: function (text) {
        this.pattern += text
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
      generateMasks: function () {
        var patexc = []
        this.excPatterns.forEach(function(pattern){
          patexc.push(pattern['text'])
        })
        var patinc = []
        this.incPatterns.forEach(function(pattern){
          patinc.push(pattern['text'])
        })
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
          wordlists: this.selectedDictionaries
        })
        .then(function (response) {
          console.log(response);
        })
        .catch(function (error) {
          console.log(error);
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