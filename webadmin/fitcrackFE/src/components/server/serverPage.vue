<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container fluid>
    <v-layout>
      <v-container class="flex1">

        <fc-tile title="Server" class="mx-3 mb-5" :loading="serverInfo == null">
          <v-list single-line class="width100" v-if="serverInfo != null">
            <template v-for="(subsystem, i) in serverInfo.subsystems">
              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key">
                  {{subsystem.name}}:
                </v-list-item-action>
                <v-list-item-content>
                  <v-list-item-title class="text-xs-right error--text"
                                     v-bind:class="{'success--text': subsystem.status==='running'}">
                    {{ subsystem.status }}
                  </v-list-item-title>
                </v-list-item-content>
              </v-list-item>
              <v-divider v-if="!(serverInfo.subsystems.length === i+1)"></v-divider>
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
        <fc-tile title="CPU & Memory" class="mx-3 mb-5 maxh300 flex1">
          <fc-graph
                  id="packageGraph1"
                  :data='usageData'
                  type="job"
                  :labels="cpumemLabels"
                  :labelsText="cpumemLabelsFriendly"
          >
          </fc-graph>
        </fc-tile>

        <fc-tile title="HDD usage" class="mx-3 mb-5 maxh300 flex1">
          <fc-graph
                  id="packageGraph2"
                  :data='usageData'
                  type="job"
                  :labels="hddLabels"
                  :labelsText="hddLabelsFriendly"
          >
          </fc-graph>
        </fc-tile>

        <fc-tile title="Network usage" class="mx-3 mb-5 maxh300 flex1">
          <fc-graph
                  id="packageGraph3"
                  :data='usageData'
                  type="job"
                  :labels="netLabels"
                  :labelsText="netLabelsFriendly"
          >
          </fc-graph>
        </fc-tile>

        <v-layout class=" dateSelectCont">
          <v-spacer></v-spacer>
          <v-flex xs5 @click="fullDate = false">
            <v-radio-group row class="dateSelect" v-model="hoursBefore" @change="loadData">
              <v-radio value="1" label="last hour" :disabled="fullDate"></v-radio>
              <v-radio value="3" label="last 3 hours" :disabled="fullDate"></v-radio>
              <v-radio value="6" label="last 6 hours" :disabled="fullDate"></v-radio>
            </v-radio-group>
          </v-flex>
          <v-flex xs6 @click="fullDate = true" class="pl-2">
            <v-layout row>
              <v-flex xs2>
                <v-subheader class="textBottom">From:</v-subheader>
              </v-flex>
              <v-flex xs4>
                <v-text-field
                        @input="loadData"
                        :disabled="!fullDate"
                        v-model="fromDate"
                        text
                        single-line
                        mask="date-with-time"
                        :placeholder="this.$moment().format('DD/MM/YYYY HH:MM')"
                ></v-text-field>
              </v-flex>
              <v-flex xs2>
                <v-subheader class="textBottom">To:</v-subheader>
              </v-flex>
              <v-flex xs4>
                <v-text-field
                        @input="loadData"
                        :disabled="!fullDate"
                        v-model="toDate"
                        text
                        single-line
                        mask="date-with-time"
                        :placeholder="this.$moment().format('DD/MM/YYYY HH:MM')"
                ></v-text-field>
              </v-flex>
            </v-layout>
          </v-flex>
        </v-layout>
      </v-container>
    </v-layout>
  </v-container>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import graph from '@/components/graph/fc_usage_graph'

  export default {
    name: "serverPage",
    components: {
      'fc-tile': tile,
      'fc-graph': graph,
    },
    mounted: function () {
      this.loadData();
      this.interval = setInterval(function () {
        this.loadData()
      }.bind(this), 15000)
    },
    methods: {
      loadData: function () {

        var fromDate = '';
        var toDate = '';

        if (this.fullDate) {
          if (this.fromDate.length === 12) {
            fromDate = this.$moment(this.fromDate, 'DDMMYYYYHHmm').format('YYYY-M-D H:mm:ss');
          } else {
            return
          }
          if (this.toDate.length === 12) {
            toDate = this.$moment(this.toDate, 'DDMMYYYYHHmm').format('YYYY-M-D H:mm:ss');
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
        fromDate: this.$moment().format('DDMMYYYYHHmm'),
        toDate: this.$moment().format('DDMMYYYYHHmm'),
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
