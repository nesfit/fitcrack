<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container fluid>
      <v-select v-model="selectedHost" :items="hosts" label="Host name" item-text="name" item-value="id" v-on:change="hostSelected()"/>
        <v-alert type="error" v-if="selectedHost == null">
          Select host to monitor
        </v-alert>
        <v-row>
          <v-col cols="8">
        <v-container v-if="selectedHost">
            <fc-tile
            title="CPU & Memory"
            icon="mdi-memory"
            class="mx-3 mb-5"
            >
            <fc-graph
                id="jobGraph1"
                :data="usageData"
                type="job"
                :labels="cpumemLabels"
                :labels-text="cpumemLabelsFriendly"
            />
            </fc-tile>
            <div v-for="i in deviceCount" v-bind:key="i">
            <fc-tile v-if="dataObtainedForDevices >= i"
            :title="'OpenCL device (' + i +')'"
            icon="mdi-gauge"
            class="mx-3 mb-5"
            >
            <fc-graph
                :id="'jobGraph'+i+1"
                :data="deviceUsage[i]"
                type="job"
                :labels="deviceLabels"
                :labels-text="deviceLabelsFriendly"
            />
            </fc-tile>
            </div>
        </v-container>
        </v-col>
        <v-col cols="4">
        <v-container v-if="platforms != null">
          <fc-tile title="Detected platforms">
            <v-divider/>
              <template v-for="platform in platforms">
                <v-row><v-col class="bold" >Platform </v-col><v-col  md="auto">{{ platform.name }}</v-col></v-row>
                <v-row><v-col class="bold" >Version </v-col><v-col  md="auto">{{ platform.version }}</v-col></v-row>
                <template v-for="dvcs in platform.platformDevices">
                  <v-row>
                    <v-col class="bold">{{ dvcs.type }}</v-col>
                    <v-col md="auto">{{ dvcs.name }}</v-col>
                  </v-row>
                </template>
                <v-divider/>
              </template>
          </fc-tile>
        </v-container>
        </v-col>
        </v-row>
  </v-container>
</template>

<script>
import tile from '@/components/tile/fc_tile'
import graph from '@/components/graph/fc_usage_graph'

export default {
    components: {
      'fc-tile': tile,
      'fc-graph': graph
    },
    data: function () {
      return {
        deviceCount:0,
        hosts:null,
        selectedHost:null,
        usageData: null,
        cpumemLabels: ['cpu_utilization', 'memory_utilization'],
        cpumemLabelsFriendly: ['CPU [%]', 'RAM [%]'],
        deviceLabels: ['utilization', 'temperature'],
        deviceLabelsFriendly: ['Utilization [%]', 'Temperature [°C]'],
        deviceUsage:[],
        platforms: null,
        dataObtainedForDevices:0
      }
    },
    watch: {
      deviceCount: function () {
        this.getDeviceStats();
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
          this.axios.get(this.$serverAddr + '/job/'+this.$route.params.id+'/hosts').then((response) => {
          console.log('/job/'+this.$route.params.id+'/hosts', response.data.items);
          this.hosts = response.data.items;   
        });
        if (this.selectedHost !== null)
          this.hostSelected();
      },
      hostSelected: function () {
          var dvcCount = 0;
          this.axios.get(this.$serverAddr + '/job/'+this.$route.params.id+'/hwMon/'+this.selectedHost+'/systemStats').then((response) => {
          console.log('/job/'+this.$route.params.id+'/hwMon/'+this.selectedHost+'/systemStats', response.data.items);
          this.usageData = response.data.items;
        });
        this.axios.get(this.$serverAddr + '/job/'+this.$route.params.id+'/hwMon/'+this.selectedHost+'/deviceCount').then((response) => {
          console.log('/job/'+this.$route.params.id+'/hwMon/'+this.selectedHost+'/deviceCount', response.data.deviceCount);
          this.deviceCount = response.data.deviceCount;
        });
        this.getDeviceStats();
        this.axios.get(this.$serverAddr + '/job/'+this.$route.params.id+'/hwMon/'+this.selectedHost+'/platforms').then((response) => {
          console.log('/job/'+this.$route.params.id+'/hwMon/'+this.selectedHost+'/platforms', response.data.items);
          this.platforms = response.data.items;
        });
      },
      getDeviceStats: function () {
        for (let i = 1; i <= this.deviceCount; i++) {
            this.axios.get(this.$serverAddr + '/job/'+this.$route.params.id+'/hwMon/'+this.selectedHost+'/deviceStats/'+i).then((response) => {
            console.log('/job/'+this.$route.params.id+'/hwMon/'+this.selectedHost+'/deviceStats/'+i, response.data.items);
            this.deviceUsage[i] = response.data.items;
            this.dataObtainedForDevices++;
        });
        }
      }
    }
}
</script>

<style scoped>
  .bold {
    font-weight: bold;
  }
</style>