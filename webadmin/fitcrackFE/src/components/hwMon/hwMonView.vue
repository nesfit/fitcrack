<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="w-100 mx-2 my-2 d-flex">
    <div class="flex-grow-2 mx-2">
      <v-card>
        <v-card-title>Hosts</v-card-title>
        <v-list>
          <v-divider />
          <v-list-item-group v-model="selectedHost" v-on:change="hostSelected()" color="primary">
            <v-list-item v-for="(host, i) in hosts" :value="host.id" :key="i">
              <v-list-item-content>
                <v-list-item-title v-text="host.name" class="pr-5"/>
              </v-list-item-content>
            </v-list-item>
          </v-list-item-group>
        </v-list>
      </v-card>
    </div>

    <div class="flex-grow-1 mx-2" v-if="selectedHost">
      <v-card class="mb-4">
        <v-card-title>
          <v-icon>mdi-memory</v-icon>CPU & Memory
        </v-card-title>
        <fc-graph
          id="jobGraph1"
          :data="usageData"
          type="job"
          :labels="cpumemLabels"
          :labels-text="cpumemLabelsFriendly"
        />
      </v-card>
      <div v-for="i in deviceCount" v-bind:key="i">
        <v-card class="mb-4" v-if="dataObtainedForDevices >= i">
          <v-card-title>
            <v-icon>mdi-gauge</v-icon>
            OpenCL device ({{i}})
          </v-card-title>
          <fc-graph
            :id="'jobGraph'+i+1"
            :data="deviceUsage[i]"
            type="job"
            :labels="deviceLabels"
            :labels-text="deviceLabelsFriendly"
          />
        </v-card>
      </div>
    </div>

    <div class="flex-grow-2 mx-2" v-if="(platforms != null) && selectedHost">
      <v-card>
        <v-card-title>Detected platforms</v-card-title>

        <v-divider />

        <template v-for="platform in platforms">
          <v-row class="mx-1">
            <v-col class="bold">Platform</v-col>
            <v-col>{{ platform.name }}</v-col>
          </v-row>

          <v-row class="mx-1">
            <v-col class="bold">Version</v-col>
            <v-col>{{ platform.version }}</v-col>
          </v-row>

          <template v-for="dvcs in platform.platformDevices">
            <v-row class="mx-1">
              <v-col class="bold">{{ dvcs.type }}</v-col>
              <v-col>{{ dvcs.name }}</v-col>
            </v-row>
          </template>

          <v-divider />
        </template>
      </v-card>
    </div>
    
  </div>
</template>

<script>
import tile from "@/components/tile/fc_tile";
import graph from "@/components/graph/fc_usage_graph";

export default {
  components: {
    "fc-tile": tile,
    "fc-graph": graph
  },
  data: function() {
    return {
      deviceCount: 0,
      hosts: null,
      selectedHost: null,
      usageData: null,
      cpumemLabels: ["cpu_utilization", "memory_utilization"],
      cpumemLabelsFriendly: ["CPU [%]", "RAM [%]"],
      deviceLabels: ["utilization", "temperature"],
      deviceLabelsFriendly: ["Utilization [%]", "Temperature [°C]"],
      deviceUsage: [],
      platforms: null,
      dataObtainedForDevices: 0,
      hostSelectedBool: false,
    };
  },
  watch: {
    deviceCount: function() {
      this.getDeviceStats();
    },
    hosts: function() {
      if (!this.hostSelectedBool){
          if (Array.isArray(this.hosts) && this.hosts.length){
            this.selectedHost = this.hosts[0].id;
            this.hostSelectedBool = true;
            this.hostSelected();
          }
        }
    }
  },
  mounted: function() {
    this.loadData();
    this.interval = setInterval(
      function() {
        this.loadData();
      }.bind(this),
      10000
    );
  },
  beforeDestroy() {
    clearInterval(this.interval);
  },
  methods: {
    loadData: function() {
      this.axios
        .get(this.$serverAddr + "/job/" + this.$route.params.id + "/hosts")
        .then(response => {
          console.log("/job/"+this.$route.params.id+"/hosts",response.data.items);
          this.hosts = response.data.items;
        });
      if (this.selectedHost != null) this.hostSelected();
    },
    hostSelected: function() {
      if (this.selectedHost != null) {
        var dvcCount = 0;
        this.axios
          .get(this.$serverAddr+"/job/"+this.$route.params.id+"/hwMon/"+this.selectedHost+"/systemStats")
          .then(response => {
            console.log("/job/"+this.$route.params.id+"/hwMon/"+this.selectedHost+"/systemStats",response.data.items);
            this.usageData = response.data.items;
          });
        this.axios
          .get(this.$serverAddr+"/job/"+this.$route.params.id+"/hwMon/"+this.selectedHost+"/deviceCount")
          .then(response => {
            console.log("/job/"+this.$route.params.id+"/hwMon/"+this.selectedHost+"/deviceCount",response.data.deviceCount);
            this.deviceCount = response.data.deviceCount;
          });
        this.getDeviceStats();
        this.axios
          .get(this.$serverAddr+"/job/"+this.$route.params.id+"/hwMon/"+this.selectedHost+"/platforms")
          .then(response => {
            console.log("/job/"+this.$route.params.id+"/hwMon/"+this.selectedHost+"/platforms",response.data.items);
            this.platforms = response.data.items;
          });
      }
    },
    getDeviceStats: function() {
      if (this.selectedHost != null) {
        for (let i = 1; i <= this.deviceCount; i++) {
          this.axios
            .get(this.$serverAddr +"/job/"+this.$route.params.id+"/hwMon/"+this.selectedHost + "/deviceStats/"+i)
            .then(response => {
              console.log("/job/"+this.$route.params.id+"/hwMon/"+this.selectedHost +"/deviceStats/"+i,response.data.items);
              this.deviceUsage[i] = response.data.items;
              this.dataObtainedForDevices++;
            });
        }
      }
    }
  }
};
</script>

<style scoped>
.bold {
  font-weight: bold;
}
</style>