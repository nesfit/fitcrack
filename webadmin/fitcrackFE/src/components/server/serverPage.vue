<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container fluid>
    <v-row>
      <v-container class="flex1">
        <fc-tile
          title="Server"
          class="mx-3 mb-5"
          :loading="serverInfo == null"
        >
          <v-list
            v-if="serverInfo != null"
            single-line
            class="width100"
          >
            <template v-for="(subsystem, i) in serverInfo.subsystems">
              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key">
                  {{ subsystem.name }}:
                </v-list-item-action>
                <v-list-item-content>
                  <v-list-item-title
                    class="text-right error--text"
                    :class="{'success--text': subsystem.status==='running'}"
                  >
                    {{ subsystem.status }}
                  </v-list-item-title>
                </v-list-item-content>
              </v-list-item>
              <v-divider v-if="!(serverInfo.subsystems.length === i+1)" />
            </template>
            <!--                        <v-list-item class="px-2 py-1 ">-->
            <!--                            <v-list-item-content class="">-->
            <!--                                <v-list-item-title class="text-xs-center height60">-->
            <!--                                    <v-btn @click="operation('start')" text outlined color="success">Start</v-btn>-->
            <!--                                    <v-btn @click="operation('stop')" text outlined color="error">Stop</v-btn>-->
            <!--                                    <v-btn @click="operation('restart')" text outlined color="info">Restart</v-btn>-->
            <!--                                </v-list-item-title>-->
            <!--                            </v-list-item-content>-->
            <!--                        </v-list-item>-->
          </v-list>
        </fc-tile>
      </v-container>
      <v-container class="flex2">
        <fc-tile
          title="CPU & Memory"
          icon="mdi-memory"
          class="mx-3 mb-5 maxh300 flex1"
        >
          <fc-graph
            id="jobGraph1"
            :data="usageData"
            type="job"
            :labels="cpumemLabels"
            :labels-text="cpumemLabelsFriendly"
          />
        </fc-tile>

        <fc-tile
          title="Disk usage"
          icon="mdi-harddisk"
          class="mx-3 mb-5 maxh300 flex1"
        >
          <fc-graph
            id="jobGraph2"
            :data="usageData"
            type="job"
            :labels="hddLabels"
            :labels-text="hddLabelsFriendly"
          />
        </fc-tile>

        <fc-tile
          title="Network usage"
          icon="mdi-ethernet"
          class="mx-3 mb-5 maxh300 flex1"
        >
          <fc-graph
            id="jobGraph3"
            :data="usageData"
            type="job"
            :labels="netLabels"
            :labels-text="netLabelsFriendly"
          />
        </fc-tile>

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
    </v-row>
  </v-container>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import graph from '@/components/graph/fc_usage_graph'
  import dtPicker from '@/components/picker/datetime'

  export default {
    name: "ServerPage",
    components: {
      'fc-tile': tile,
      'fc-graph': graph,
      dtPicker
    },
    data: function () {
      return {
        serverInfo: null,
        usageData: null,
        cpumemLabels: ['cpu', 'ram'],
        cpumemLabelsFriendly: ['CPU [%]', 'RAM [%]'],
        hddLabels: ['hdd_read', 'hdd_write'],
        hddLabelsFriendly: ['HDD Read [Kb/s]', 'HDD Write [Kb/s]'],
        netLabels: ['net_recv', 'net_sent'],
        netLabelsFriendly: ['Net receive [Kb/s]', 'Net sent [Kb/s]'],
        loading: false,
        fullDate: false,
        hoursBefore: '1',
        fromDate: this.$moment().format('YYYY-MM-DDTHH:mm:ss'),
        toDate: this.$moment().format('YYYY-MM-DDTHH:mm:ss'),
      }
    },
    mounted: function () {
      this.loadData();
      this.interval = setInterval(function () {
        this.loadData()
      }.bind(this), 15000)
    },
    beforeDestroy () {
      clearInterval(this.interval)
    },
    methods: {
      loadData: function () {

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

        this.axios.get(this.$serverAddr + '/serverInfo/getUsageData?from_date=' + fromDate + '&to_date=' + toDate).then((response) => {
          console.log("/getUsageData", response.data.items);
          this.usageData = response.data.items;
        });

        this.axios.get(this.$serverAddr + '/serverInfo/info').then((response) => {
          this.serverInfo = response.data
        });

      },
      operation: function (oper) {
        this.loading = true
        this.axios.get(this.$serverAddr + '/serverInfo/control?operation=' + oper).then((response) => {
          this.loadData()
          this.loading = false
        }).catch((error) => {
          this.loading = false
        })
      },
      yesNo: function (val) {
        return val ? 'Yes' : 'No'
      }
    }
  }
</script>

<style scoped>
  .maxh300 {
    max-height: 300px;
  }

  .flex1 {
    flex: 1;
  }

  .flex2 {
    flex: 2;
  }

  .height60 {
    height: 100px;
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

  .key {
    font-weight: bold;
  }
</style>
