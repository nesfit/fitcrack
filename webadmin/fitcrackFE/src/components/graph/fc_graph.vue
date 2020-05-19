<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="graphContainer">
    <div
      v-if="data === null"
      class="loading"
    >
      <v-progress-circular
        indeterminate
        color="primary"
      />
    </div>
    <div
      v-else-if="data.data.length === 0"
      class="loading"
    >
      <h1>No data</h1>
    </div>
    <div v-else>
      <div v-if="data.type === 'line'">
        <line-chart
          :id="id"
          class="d-flex graph line"
          :data="data.data"
          :xkey="data.axies.x"
          :ykeys="data.axies.y"
          :labels="data.labels"
          grid="true"
          :ymax="type === 'job' ? '100' : 'auto'"
          hide-hover="false"
          :resize="true"
          :post-units="units"
          :smooth="false"
          :hover-callback="type === 'job' ? hoverCallbackJob : hoverCallbackWorkunit"
          :line-colors="palette"
        />
      </div>
      <div v-else-if="data.type === 'pie'">
        <donut-chart
          :id="id || 'pie'"
          class="d-flex graph donut"
          :data="data.data"
          :resize="true"
          :colors="palette"
        />
      </div>
    </div>
  </div>
</template>

<script>
  import Raphael from 'raphael/raphael'
  import {DonutChart, LineChart} from 'vue-morris'

  global.Raphael = Raphael;

  export default {
    name: "FcGraph",
    components: {
      LineChart,
      DonutChart
    },
    props: ['data', 'id', 'units', 'type'],
    data: function () {
      return {
        i: 1,
        palette: ['#f24b78', '#605f5e', '#1d3461', '#48beff', 
                  '#b300ec', '#a9d2f9', '#ffd8b1', '#008080', 
                  '#d2f53c', '#ffd700', '#ff0051', '#65157f', 
                  '#ff4300', '#193bdf', '#afe31b', '#c28f62']
      }
    },
    methods: {
      hoverCallbackJob: function (index, options, content, row) {
        if (Object.keys(row).length < 2)
          return
        var jobId = Object.keys(row)[0];
        var progress = row[jobId];
        var time = this.$moment(row['time']).format('DD.MM.YYYY HH:mm');
        var jobName = '';

        for (let i = 0; i < options.ykeys.length; i++) {
          if (options.ykeys[i] === jobId) {
            jobName = options.labels[i];
            break
          }
        }

        return '<b>' + time + '</b></br> Job <a href="/jobs/' + jobId + '"> ' +
          jobName + '</a> ' + progress + '%'
      },
      hoverCallbackWorkunit: function (index, options, content, row) {
        if (Object.keys(row).length < 2)
          return
        var hostIdentifier = Object.keys(row)[1]
        var hostId = hostIdentifier.split('_')[0]
        var hashes = row[hostIdentifier];
        var time = this.$moment(row['time']).format('DD.MM.YYYY HH:mm');
        var hostName = '';

        for (let i = 0; i < options.ykeys.length; i++) {
          if (options.ykeys[i] === hostIdentifier) {
            hostName = options.labels[i];
            break
          }
        }

        return '<b>' + time + '</b></br> Host <a href="/hosts/' + hostId + '"> ' +
          hostName + '</a> ' + hashes + ' hashes'
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
    font-weight: 200;
  }

  .donut.graph {
    height: 250px
  }
</style>

<style>
  .donut.graph text {
    font-family: Roboto !important;
  }
  .theme--dark .donut.graph text {
    fill: #efefef;
  }
</style>
