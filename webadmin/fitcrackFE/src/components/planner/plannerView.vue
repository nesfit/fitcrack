<!--
   * Author : David Brandšteter xbrand13
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max1000">
    <fc-tile
      title="Planner"
      class="ma-2"
      :icon="$route.meta.icon"
    >
    <v-text-field
          v-model="name"
          outlined
          autofocus
          required
          label="Name"
    />

    <v-expansion-panels class="margin">
      <v-expansion-panel>
        <v-expansion-panel-header>
          <span class="d-flex align-center">
                <v-icon left>
                  mdi-book-lock
                </v-icon>
                <span class="text-h6">Select Hashlist</span>
              </span>
        </v-expansion-panel-header>
        <v-expansion-panel-content>
          <v-container>
            <v-row>
              <v-col>
                <v-btn
                  large block
                  color="primary"
                  :to="{name: 'createHashlist', query: {attach: `${name} Hashlist`}}"
                >
                  <v-icon left>mdi-book-plus</v-icon>
                  Create new input hashlist
                </v-btn>
              </v-col>
            </v-row>
            <v-row>
              <v-col>
                <div class="text-center text-overline text--secondary">or attach an existing one</div>
                <HashlistSelector
                v-model="hashListId"
                />
              </v-col>
            </v-row>
          </v-container>
        </v-expansion-panel-content>
      </v-expansion-panel>
    </v-expansion-panels>

    <div class="d-flex">
      <v-text-field
      v-model="time.days"
      class="mx-2 time-width"
      type = "number"
      label="Days"
      min="0"
      />
      <v-text-field
      v-model="time.hours"
      class="mx-2 time-width"
      type = "number"
      label="Hours"
      min="0"
      max="23"
      />
      <v-text-field
      v-model="time.minutes"
      class="mx-2 time-width"
      type = "number"
      label="Minutes"
      min="0"
      max="59"
      />
    </div>


    <v-expansion-panels class="margin">
      <v-expansion-panel>
        <v-expansion-panel-header>
          <span class="d-flex align-center">
                <v-icon left>
                  mdi-desktop-classic
                </v-icon>
                <span class="text-h6">Select Hosts</span>
              </span>
        </v-expansion-panel-header>
        <v-expansion-panel-content>
          <v-container>
            <v-subheader>Select which hosts to distribute workunits to</v-subheader>
              <div class="scroller">
              <host-selector
                v-model="hosts"
                select-all
                auto-refresh
              />
              </div>
            <v-subheader><b>({{ hosts.length }} selected)</b></v-subheader>
          </v-container>
        </v-expansion-panel-content>
      </v-expansion-panel>
    </v-expansion-panels>

    <v-text-field
      v-model="crackSpeed"
      class="mx-2 speed-width"
      type="number"
      label="Cracking Speed (Hs/s)"
      step="1000000"
      min="0"
      />
      <v-btn
        class="d-inline-block"
        color="primary"
        @click.native.stop="getSpeedFromHostBenchmark"
      >
      Get Speed from Benchmarks
      </v-btn>
    <div class="fill_div">
      <v-checkbox 
      label="Fill rest with brute-force attack"
      v-model="fill"
      >
      </v-checkbox>
      <div class="fill_div">
        <v-text-field
          :disabled="!fill"
          v-model="password_len_min"
          class="mx-2 length-width"
          type ="number"
          label="Min Password Lenght"
          min="0"
        />
        <v-text-field
          :disabled="!fill"
          v-model="password_len_max"
          class="mx-2 length-width"
          type ="number"
          label="Max Password Lenght"
          min="0"
        />
      </div>
    </div>
    <div class="text-center pa-3">
    <v-btn
        :disabled="loading"
        class="d-inline-block"
        color="primary"
        @click.native.stop="submit"
    >
    Submit
    </v-btn>
    </div>
  </fc-tile>
  </v-container>
</template>



<script>
  import hostSelector from '@/components/selector/hostSelector.vue'
  import HashlistSelector from '@/components/selector/hashlistSelector.vue'
  import tile from '@/components/tile/fc_tile.vue'
  import {mapTwoWayState} from 'spyfu-vuex-helpers'
  import {twoWayMap} from '@/store'
  export default {
    name: "PlannerView",
    components: {
      'fc-tile': tile,
      'host-selector': hostSelector,
      HashlistSelector
    },
    data: function () {
      return {
        loading: false,
        name: "Planned Batch",
        fill: false,
        password_len_min: 3,
        password_len_max: 6,
        time: {
          days: 0,
          hours: 0,
          minutes: 0
        },
        hashTypeName: "",
        hashTypeCode: -1,
        hashTypes: [],
        hashList: [],
        hostsInfo: [],
        hostsIds: [],
        hostsBenchmarks: {},
        benchmarkPowerSum: 0,

        totalSeconds: 0,
        crackSpeed: 1000000,
        totalCrackSpeed: 0,
        availableKeySpace: 0,

        addr: this.$serverAddr,
      }
    },
    computed: {
      ...mapTwoWayState('jobForm', twoWayMap([
        'hashListId', 'hosts'
      ]))
    },
    mounted: function () {
      this.getHashTypes()
      this.getHostsInfo()
    },
    methods: {
      submit() {
        if (this.name === '') {
          this.$error('Job name can not be empty.')
          return
        }

        if (!this.hashListId) {
          this.$error('No Hashlist selected')
          return
        }

        if (this.time.days < 0 || this.time.hours < 0 || this.time.minutes < 0) {
          this.$error('Time cannot be negative')
          return
        }

        if (this.time.days == 0 && this.time.hours == 0 && this.time.minutes == 0) {
          this.$error('Cannot plan attacks with no time')
          return
        }

        if (this.fill && this.password_len_min > this.password_len_max) {
          this.$error('Minimal password lenght cannot be higher than maximum.')
          return
        }

        this.loading = true
        
        for (let i = 0; i < this.hosts.length; i++) {
            this.hostsIds.push(this.hosts[i].id)
        }

        this.axios.post(this.$serverAddr + '/planner', {
          'days': this.time.days,
          'hours': this.time.hours,
          'minutes': this.time.minutes,
          'cracking_speed': this.crackSpeed,
          'name': this.name,
          'hashList': this.hashListId,
          'hostsIds': this.hostsIds.join(","),
          'fill': this.fill,
          'password_len_min': this.password_len_min,
          'password_len_max': this.password_len_max
        }).then((response) => {
          const batchId = response.data.batch_id
          const routeUrl = '/batches/' + batchId
          this.$router.push(routeUrl)
        })
      },

      getSpeedFromHostBenchmark: async function () {
        this.hostsBenchmarks = {}

        if (this.hosts.length === 0) {
          this.$error('No hosts selected')
          return
        }

        if (!this.hashListId) {
          this.$error('No Hashlist selected')
          return
        }

        const hash_list = await this.axios.get(this.$serverAddr + '/hashlist/' + this.hashListId);
        this.hashTypeName = hash_list.data.hash_type_name

        for (let i = 0; i < this.hosts.length; i++) {
          const response = await this.axios.get(this.$serverAddr + '/hosts/' + this.hosts[i].id + '/benchmarks');
          this.hostsBenchmarks[this.hosts[i].id] = response.data.items
        }

        console.log(this.hostsBenchmarks)

        if (this.hostsBenchmarks.length == 0) {
          this.$error('No benchmarks for one of the selected host found.')
          return
        }

        this.hashTypeCode = this.hashTypes.find(type => type.name === this.hashTypeName).code
        this.benchmarkPowerSum = 0

        for (let i = 0; i < this.hosts.length; i++) {
          const FoundSpeed = this.hostsBenchmarks[this.hosts[i].id].find(benchmark => benchmark.attack_mode == 0 && benchmark.hash_type == this.hashTypeCode)
          if (FoundSpeed == null) {
            this.$error('No benchmarks for one of the selected attack/hash type found.')
            return
          }
        
          this.benchmarkPowerSum += FoundSpeed.power
        }

        this.crackSpeed = this.benchmarkPowerSum
      },
      getHashTypes: function () {
        this.axios.get(this.$serverAddr + '/hashcat/hashTypes').then((response) => {
          this.hashTypes = response.data.hashtypes
        })
      },
      getHostsInfo: function () {
        this.axios.get(this.$serverAddr + '/hosts').then((response) => {
          this.hostsInfo = response.data
        })
      }
    }
  }
</script>

<style scoped>
  .max1000 {
    max-width: 1000px;
  }

  .time-width {
    max-width: 75px;
  }

  .speed-width {
    max-width: 250px;
  }

  .fill_div {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
  }

  .length-width {
    max-width: 150px;
  }

  .margin {
    margin-top: 1rem;
    margin-bottom: 1rem;
  }

</style>