<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="graphContainer">
    <div v-if="data === null" class="loading">
      <v-progress-circular indeterminate color="primary"></v-progress-circular>
    </div>
    <div v-else-if="data.data.length === 0" class="loading">
      <h1>No data</h1>
    </div>
    <div v-else>
      <div v-if="data.type === 'line'">
        <line-chart
          :id="id"
          class="d-flex graph"
          :data='data.data'
          :xkey='data.axies.x'
          :ykeys="data.axies.y"
          :labels='data.labels'
          grid="true"
          :ymax="type === 'job' ? '100' : 'auto'"
          hide-hover="false"
          resize="true"
          :post-units="units"
          :smooth="false"
          :hoverCallback="type === 'job' ? hoverCallbackPackage : hoverCallBackJob"
          :line-colors="['#1a50a3', '#00b752', '#dc3043', '#eecd3b', '#768ea2', '#b300ec', '#a9d2f9', '#ffd8b1', '#008080', '#d2f53c']"

        >
        </line-chart>
      </div>
      <div v-else-if="data.type === 'pie'">
        <donut-chart
          :id="id"
          id="pie"
          class="d-flex graph"
          :data='data.data'
          resize="true"
          :colors="['#1a50a3', '#00b752', '#dc3043', '#eecd3b', '#768ea2', '#b300ec', '#a9d2f9', '#ffd8b1', '#008080', '#d2f53c']"
        >
        </donut-chart>
      </div>
    </div>
  </div>
</template>

<script>
  import Raphael from 'raphael/raphael'
  import {DonutChart, LineChart} from 'vue-morris'

  global.Raphael = Raphael;

  export default {
    name: "fc_graph",
    props: ['data', 'id', 'units', 'type'],
    components: {
      LineChart,
      DonutChart
    },
    methods: {
      hoverCallbackPackage: function (index, options, content, row) {
        if (Object.keys(row).length < 2)
          return
        var packageId = Object.keys(row)[0];
        var progress = row[packageId];
        var time = this.$moment(row['time']).format('DD.MM.YYYY HH:mm');
        var packageName = '';

        for (let i = 0; i < options.ykeys.length; i++) {
          if (options.ykeys[i] === packageId) {
            packageName = options.labels[i];
            break
          }
        }

        return '<b>' + time + '</b></br> Job <a href="/jobs/' + packageId + '"> ' +
          packageName + '</a> ' + progress + '%'
      },
      hoverCallBackJob: function (index, options, content, row) {
        if (Object.keys(row).length < 2)
          return
        var hostIdentifier = Object.keys(row)[1]
        var hostId = hostIdentifier.split('_')[0]
        console.log(hostId)
        var time = this.$moment(row['time']).format('DD.MM.YYYY HH:mm');
        var hostName = '';

        for (let i = 0; i < options.ykeys.length; i++) {
          if (options.ykeys[i] === hostIdentifier) {
            hostName = options.labels[i];
            break
          }
        }

        return '<b>' + time + '</b></br> Host <a href="/hosts/' + hostId + '"> ' +
          hostName + '</a>'
      }
    },
    data: function () {
      return {
        i: 1
      }
    }
  }
</script>

<style scoped>
  .graphContainer {
    width: 100%;
  }

  .loading {
    text-align: center;
    padding: 20px;
    vertical-align: middle;
    color: #3e3e3e;
    font-weight: 200;
  }
</style>
