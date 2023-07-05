<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs
      v-if="data != null"
      :items="
        [
          { text: 'Hosts', to: { name : 'hosts' }, exact: true },
          { text: data.domain_name }
        ]"
      divider="/"
    />

    <v-container>
      <fc-tile
        title="Info"
        :loading="data==null"
        class="mb-5"
      >
        <v-list v-if="data != null">
          <v-list-item>
            <v-list-item-icon>
              <v-icon>mdi-desktop-classic</v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title>
                {{ data.domain_name }}
              </v-list-item-title>
              <v-list-item-subtitle>
                User: {{ data.user.name }}
              </v-list-item-subtitle>
            </v-list-item-content>
          </v-list-item>
          <v-list-item>
            <v-list-item-icon>
              <v-icon>{{ getOsIcon(data.os_name) }}</v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title>
                {{ data.os_name }}
              </v-list-item-title>
            </v-list-item-content>
          </v-list-item>
          <v-list-item>
            <v-list-item-icon>
              <v-icon>mdi-memory</v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title>
                {{ data.p_model }}
              </v-list-item-title>
            </v-list-item-content>
          </v-list-item>
        </v-list>
      </fc-tile>

      
      <fc-tile
        title="Devices"
        :loading="data === null"
        class="mb-5"
      >
        <v-list v-if="data != null">
          <v-list-item v-for="device in data.devices" :key="device.id">
            <v-list-item-icon>
              <v-icon v-if="device.device_info[0].temperature > 100">mdi-fire</v-icon>
              <v-icon v-else>{{ device.type === 'GPU' ? 'mdi-expansion-card' : 'mdi-expansion-card-variant' }}</v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title>
                {{ device.name }}
              </v-list-item-title>
              <v-list-item-subtitle v-if="$moment.utc(device.device_info[0].time).isBefore($moment.utc().subtract(10, 'minutes'))">
                Inactive – no current data.
              </v-list-item-subtitle>
              <v-list-item-subtitle v-else>
                Speed: {{ device.device_info[0].speed}} H/s<span v-show="device.device_info[0].temperature >= 0">, Temp: {{ device.device_info[0].temperature}} °C, Util: {{ device.device_info[0].utilization}} %</span>
              </v-list-item-subtitle>
              <div class="device-charts">
                <res-mon
                  class="chart"
                  :usage-data="device.device_info.slice(0,30)"
                  :metrics="['speed']"
                  :unit-callback="x => `${x} H/s`"
                />
                <res-mon
                  v-show="device.device_info[0].temperature >= 0"
                  class="chart"
                  :usage-data="device.device_info.slice(0,30)"
                  :metrics="['utilization', 'temperature']"
                  :max="100"
                  :right-scale="true"
                  :max-right="100"
                />
              </div>
            </v-list-item-content>
          </v-list-item>
        </v-list>
      </fc-tile>


      <fc-tile
        title="Jobs"
        class="mb-5"
        :loading="data === null"
      >
        <v-data-table
          v-if="data !== null"
          :headers="jobHeaders"
          :items="data.jobs"
          :footer-props="{itemsPerPageOptions: [10,25,50,100], itemsPerPageText: 'Jobs per page'}"
        >
          <!-- Job name cell -->
          <template v-slot:item.name="{ item }">
            <router-link
              :to="{ name: 'jobDetail', params: { id: item.id } }"
              class="middle"
            >
              {{ item.name }}
            </router-link>
          </template>
          <!-- Status text cell -->
          <template v-slot:item.status="{ item }">
            <v-tooltip top>
              <template v-slot:activator="{ on }">
                <span
                  :class="item.status_type + '--text'"
                  v-on="on"
                >
                  {{ item.status_text }}
                </span>
              </template>
              <span>{{ item.status_tooltip }}</span>
            </v-tooltip>
          </template>
          <!-- Progress indicator cell -->
          <template v-slot:item.progress="{ item }">
            <div class="d-flex align-center justify-end">
              <span class="mr-2">{{ progressToPercentage(item.progress) }}</span>
              <v-progress-circular
                size="18"
                :width="3"
                :rotate="270"
                color="primary"
                class="jobProgress"
                :value="item.progress"
              />
            </div>
          </template>
          <!-- Date added cell -->
          <template v-slot:item.time="{ item }">
            {{ $moment.utc(item.time).local().format('D.M.YYYY H:mm:ss') }}
          </template>
          <!-- Action buttons cell -->
          <template v-slot:item.actions="{ item }">
            <div class="d-flex justify-end actionsBtns">
              <v-tooltip top>
                <template v-slot:activator="{ on }">
                  <v-btn
                    icon
                    class="mx-0"
                    :disabled="item.status !== '0'"
                    v-on="on"
                    @click="operateJob(item.id, 'start')"
                  >
                    <v-icon color="success">
                      mdi-play-circle
                    </v-icon>
                  </v-btn>
                </template>
                <span>Start job</span>
              </v-tooltip>
              <v-tooltip top>
                <template v-slot:activator="{ on }">
                  <v-btn
                    icon
                    class="mx-0"
                    :disabled="item.status !== '10'"
                    v-on="on"
                    @click="operateJob(item.id, 'stop')"
                  >
                    <v-icon color="error">
                      mdi-pause-circle
                    </v-icon>
                  </v-btn>
                </template>
                <span>Stop job</span>
              </v-tooltip>
              <v-tooltip top>
                <template v-slot:activator="{ on }">
                  <v-btn
                    icon
                    class="mx-0"
                    :disabled="item.status >= 10"
                    v-on="on"
                    @click="operateJob(item.id, 'restart')"
                  >
                    <v-icon color="info">
                      mdi-restart
                    </v-icon>
                  </v-btn>
                </template>
                <span>Restart job</span>
              </v-tooltip>
            </div>
          </template>
        </v-data-table>
      </fc-tile>
    </v-container>
  </div>
</template>

<script>
  import fmt from '@/assets/scripts/numberFormat'
  import iconv from 'iconv-lite'
  import tile from '@/components/tile/fc_tile.vue'
  import ResMon from '@/components/chart/resourceMonitor.vue'
  export default {
    name: "HostDetailView",
    components: {
      'fc-tile': tile,
      'res-mon': ResMon
    },
    data: function () {
      return {
        data: null,
        jobHeaders: [
          {
            text: 'Name',
            align: 'start',
            value: 'name'
          },
          {text: 'Attack type', value: 'attack', align: 'end'},
          {text: 'Status', value: 'status', align: 'end'},
          {text: 'Progress', value: 'progress', align: 'end'},
          {text: 'Added', value: 'time', align: 'end'},
          {text: 'Actions', value: 'actions', sortable: false, align: 'end'}
        ],
        jobs_statuses: [
          {
            'text': 'ready',
            'code': 0
          },
          {
            'text': 'finished',
            'code': 1
          },
          {
            'text': 'exhausted',
            'code': 2
          },
          {
            'text': 'malformed',
            'code': 3
          },
          {
            'text': 'timeout',
            'code': 4
          },
          {
            'text': 'running',
            'code': 10
          },
          {
            'text': 'finishing',
            'code': 12
          }
        ],
      }
    },
    mounted: function () {
      this.loadData()
    },
    methods: {
      fmt,
      loadData: function () {
        this.axios.get(this.$serverAddr + '/hosts/' + this.$route.params.id).then((response) => {
          this.data = response.data;
        })
      },
      getOsIcon (os) {
        if (/(windows|microsoft)/ig.test(os)) {
          return 'mdi-windows'
        } else if (/(linux)/ig.test(os)) {
          return 'mdi-linux'
        } else if (/(apple|mac)/ig.test(os)) {
          return 'mdi-apple'
        } else {
          return 'mdi-application'
        }
      },
      progressToPercentage: function (progress) {
        return progress.toFixed() + '%'
      },
      operateJob: function (id, operation) {
        this.axios.get(this.$serverAddr + '/job/' + id + '/action', {
          params: {
            'operation': operation
          }
        }).then((response) => {
          console.log(response.data);
          this.loadJobs()
        })
      }
    }

  }
</script>

<style scoped>
  .dictContentContainer {
    max-width: 600px;
  }

  .width100 {
    width: 100%;
  }

  .max800 {
    max-width: 800px;
  }

  .device-charts {
    display: flex;
    flex-wrap: wrap;
  }

  .device-charts .chart {
    flex: 1 0 50%;
    min-width: 400px;
  }

</style>
