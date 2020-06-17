<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container fluid>
      <v-select v-model="selectedHost" :items="hosts" label="Host name" item-text="name" item-value="id" v-on:change="hostSelected()"/>
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
            <div v-for="i in deviceCount" v-bind:key="i">
            <fc-tile v-if="dataObtainedForDevices >= i"
            :title="'OpenCL device (' + i +')'"
            icon="mdi-gauge"
            class="mx-3 mb-5 maxh300 flex1"
            >
            <fc-graph
                :id="'jobGraph1'+i"
                :data="deviceUsage[i]"
                type="job"
                :labels="deviceLabels"
                :labels-text="deviceLabelsFriendly"
            />
            </fc-tile>
            </div>
        </v-container>
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
        dataObtainedForDevices:0
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