
<template>
  <v-container fluid>
    <v-row>
      <v-col>
        <!-- Jobs overview -->
        <v-card 
          v-if="jobsInfo"
          class="mb-4" 
        >
          <v-card-title>
            <router-link to="/jobs">
              Jobs
            </router-link>
          </v-card-title>
          <v-card-subtitle class="text-start">
            Status overview
          </v-card-subtitle>
          <v-card-text class="d-flex flex-wrap justify-space-between">
            <job-tile 
              v-for="(item, i) in jobsInfo" 
              :key="i"
              :item="item"
            />
          </v-card-text>
        </v-card>
        <!-- Latest jobs list -->
        <v-card 
          v-if="lastJobs"
          class="mb-4"
          min-width="300"
        >
          <v-card-title>Latest activity</v-card-title>
          <v-card-subtitle class="text-start">
            Last <b>{{ lastJobs.length }}</b> jobs
          </v-card-subtitle>
          <v-list
            v-if="lastJobs != null"
            dense
          >
            <v-list-item 
              v-for="(item, i) in lastJobs"
              :key="i"
              :to="'/jobs/' + item.id"
            >
              <v-list-item-content>
                <v-list-item-title class="text-start">{{ item.name }}</v-list-item-title>
              </v-list-item-content>
              <v-list-item-content>
                <v-list-item-title
                  :class="item.status_type + '--text'"
                  class="text-end"
                >
                  <v-tooltip top>
                    <template v-slot:activator="{ on }">
                      <span v-on="on">
                        {{ item.status_text }} ({{ progressToPercentage(item.progress) }})
                      </span>
                    </template>
                    <span>{{ item.status_tooltip }}</span>
                  </v-tooltip>
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
          </v-list>
        </v-card>
      </v-col>
      <v-col>
        <!-- Server status -->
        <v-card 
          v-if="serverInfo && actualUsage"
          class="mb-4" 
        >
          <v-card-title>
            <router-link to="/server">
              Server status
            </router-link>
          </v-card-title>
          <v-card-subtitle class="text-start">
            <span 
              v-if="serverInfoUp < serverInfoUp + serverInfoDown"
              class="warning--text"
            >
              <b>{{ serverInfoUp }}</b> of {{ serverInfoUp + serverInfoDown }} services running
            </span>
            <span v-else>All services running</span>
          </v-card-subtitle>
          <v-card-text class="d-flex flex-wrap justify-space-between">
            <grid-tile 
              icon="mdi-memory"
              :color="usageColor(actualUsage.cpu)"
            >
              <b>CPU</b>
              <span class="display-1 mt-1">{{ actualUsage.cpu }}%</span>
            </grid-tile>

            <grid-tile 
              icon="mdi-chip"
              :color="usageColor(actualUsage.ram)"
            >
              <b>Memory</b>
              <span class="display-1 mt-1">{{ actualUsage.ram }}%</span>
            </grid-tile>

            <grid-tile icon="mdi-harddisk">
              <div>
                <b>Disk</b>
                <div class="d-flex justify-space-between">
                  <v-icon small>mdi-download</v-icon> {{ actualUsage.hdd_read }} kb/s
                </div>
                <div class="d-flex justify-space-between">
                  <v-icon small>mdi-upload</v-icon> {{ actualUsage.hdd_write }} kb/s
                </div>
              </div>
            </grid-tile>

            <grid-tile icon="mdi-ethernet">
              <div>
                <b>Network</b>
                <div class="d-flex justify-space-between">
                  <v-icon small>mdi-download</v-icon> {{ actualUsage.net_recv }} kb/s
                </div>
                <div class="d-flex justify-space-between">
                  <v-icon small>mdi-upload</v-icon> {{ actualUsage.net_sent }} kb/s
                </div>
              </div>
            </grid-tile>
          </v-card-text>
        </v-card>
        <!-- Hosts overview -->
        <v-card 
          v-if="hostsInfo"
          class="mb-4" 
        >
          <v-card-title>
            <router-link to="/hosts">
              Hosts
            </router-link>
          </v-card-title>
          <v-card-subtitle class="text-start">
            <b>{{ hostsInfo.activeHosts }}</b> of {{ hostsInfo.totalHosts }} online
          </v-card-subtitle>
          <v-card-text>
            <div class="d-flex flex-wrap justify-space-around">
              <v-icon
                v-for="i in hostsInfo.activeHosts"
                :key="'a' + i"
                color="success"
              >
                mdi-desktop-classic
              </v-icon>
              <v-icon
                v-for="i in hostsInfo.inactiveHosts"
                :key="'i' + i"
                class="inactiveHost"
              >
                mdi-desktop-classic
              </v-icon>
            </div>
          </v-card-text>
        </v-card>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <v-card>
          <v-card-title>Job activity</v-card-title>
          <fc-graph
            id="jobGraph"
            :data="jobGraph"
            type="job"
          />
        </v-card>
      </v-col>
      <v-col>
        <v-card>
          <v-card-title>Host activity</v-card-title>
          <fc-graph
            id="hostGraph"
            :data="hostGraph"
            type="host"
          />
        </v-card>
      </v-col>
    </v-row>

    <v-row>
      <v-col class="d-flex">
        <v-switch
          v-model="fullDate"
          label="Custom range"
          class="mr-4"
        />
        <div 
          v-if="fullDate"
          class="d-flex"
        >
          <dt-picker
            v-model="fromDate"
            class="mr-4"
            label="From"
            @input="loadData"
          />
          <dt-picker
            v-model="toDate"
            label="To"
            @input="loadData"
          />
        </div>
        <div v-else>
          <v-radio-group
            v-model="hoursBefore"
            row
            hide-details
            class="dateSelect"
            @change="loadData"
          >
            <v-radio
              value="1"
              label="last hour"
            />
            <v-radio
              value="3"
              label="last 3 hours"
            />
            <v-radio
              value="6"
              label="last 6 hours"
            />
          </v-radio-group>
        </div>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
  import gridTile from '@/components/home/gridTile'
  import jobTile from '@/components/home/jobTile'
  import fcGraph from '@/components/graph/fc_graph'
  import { attackIcon } from '@/assets/scripts/iconMaps'
  import dtPicker from '@/components/picker/datetime'

  export default {
    name: 'HomeView',
    components: {
      gridTile,
      jobTile,
      fcGraph,
      dtPicker
    },
    data: function () {
      return {
        jobGraph: null,
        hostGraph: null,
        interval: null,
        hoursBefore: '6',
        fromDate: this.$moment().toISOString().slice(0, 16),
        toDate: this.$moment().toISOString().slice(0, 16),
        fullDate: false,
        notifications: null,
        hostsInfo: null,
        jobsInfo: null,
        serverInfo: null,
        serverInfoUp: 1,
        serverInfoDown: 1,
        lastJobs: null,
        actualUsage: {
          cpu: 0,
          ram: 0,
          hdd_read: 0,
          hdd_write: 0,
          net_recv: 0,
          net_sent: 0
        }
      }
    },
    mounted: function () {
      this.loadData();

      this.interval = setInterval(function () {
        this.loadData()
      }.bind(this), 15000)
    },
    beforeDestroy: function () {
      clearInterval(this.interval)
    },
    methods: {
      usageColor (val) {
        if (val < 50) {
          return ''
        } else if (val < 80) {
          return 'warning'
        } else {
          return 'error'
        }
      },
      yesNo: function (val) {
        return val ? 'Yes' : 'No'
      },
      progressToPercentage: function (progress) {
        return progress.toFixed() + '%'
      },
      loadData: function () {


        this.axios.get(this.$serverAddr + '/job/lastJobs')
                .then((response) => {
                  this.lastJobs = response.data.items;
                });

        this.axios.get(this.$serverAddr + '/serverInfo/actualUsageData'
        ).then((response) => {
          this.actualUsage = response.data;
        });


        var fromDate = '';
        var toDate = '';

        if (this.fullDate) {
          if (this.fromDate.length === 16) {
            fromDate = this.$moment(this.fromDate, 'YYYY-MM-DDTHH:mm').format('YYYY-M-D H:mm:ss');
          } else {
            return
          }
          if (this.toDate.length === 16) {
            toDate = this.$moment(this.toDate, 'YYYY-MM-DDTHH:mm').format('YYYY-M-D H:mm:ss');
          } else {
            return
          }
        } else {
          fromDate = this.$moment().subtract(this.hoursBefore, 'hours').format('YYYY-M-D H:mm:ss');
        }


        this.axios.get(this.$serverAddr + '/graph/jobsProgress?from_date=' + fromDate + '&to_date=' + toDate).then((response) => {
          this.jobGraph = response.data;
        });
        this.axios.get(this.$serverAddr + '/graph/hostsComputing?from_date=' + fromDate + '&to_date=' + toDate).then((response) => {
          this.hostGraph = response.data;
        });

        this.axios.get(this.$serverAddr + '/notifications?seen=false').then((response) => {
          this.notifications = response.data;
        });

        this.axios.get(this.$serverAddr + '/hosts/info').then((response) => {
          this.hostsInfo = response.data;
        });

        this.axios.get(this.$serverAddr + '/job/info').then((response) => {
          this.jobsInfo = response.data;
        });

        this.axios.get(this.$serverAddr + '/serverInfo/info').then((response) => {
          this.serverInfo = response.data.subsystems;

          const statuses = this.serverInfo.map(a => a.status);
          this.serverInfoUp = 0;
          this.serverInfoDown = 0;

          for (let i = 0; i < statuses.length; i++) {
            if (statuses[i] === "running")
              this.serverInfoUp++;
            else
              this.serverInfoDown++;
          }
        })
      }
    }
  }
</script>

<style scoped>
  .v-card__title a {
    color: inherit;
  }

  .inactiveHost {
    opacity: .2;
  }

  .textBottom {
    align-items: flex-end;
    justify-content: flex-end;
  }

  .dateSelectCont {
    width: 100%;
    text-align: right;
  }

  .dateSelect {
    display: inline-block;
    max-width: 450px;
  }

  .container {
    width: 100%;
    text-align: center;
  }

  .notifCont {
    width: 100%;
    max-height: 400px;
    overflow: auto;
  }

  .max1000 {
    max-width: 3000px;
  }

  .key {
    font-weight: bold;
  }

  .height100 {
    height: 100%;
  }

  .flex1 {
    flex: 1;
  }

  .wid20 {
    width: 20%;
  }
</style>
