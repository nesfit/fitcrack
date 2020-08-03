<template>
  <div class="container">
    <timeseries
      :chart-data="chartdata"
      :overrides="options"
      class="chart-wrapper"
    />
  </div>
</template>

<script>
import Timeseries from './types/timeseries'
import { prepareLines, plotUsage } from './helpers'

export default {
  components: { Timeseries },
  props: {
    usageData: {
      type: Array,
      default: () => []
    },
    metrics: {
      type: Array,
      default: () => []
    },
    unitCallback: {
      type: Function,
      default: x => x
    },
    min: {
      type: Number,
      default: 0
    },
    max: {
      type: Number,
      default: undefined
    }
  },
  computed: {
    chartdata () {
      return {
        datasets: prepareLines(
          this.metrics.map(metric => ({
            data: plotUsage(this.usageData, metric),
            label: this.metricLabel(metric)
          }))
        )
      }
    },
    options () {
      return {
        elements: {
          line: {
            fill: true
          },
          point: {
            radius: 0
          }
        },
        tooltips: {
          mode: "index",
          position: "nearest",
          callbacks: {
            label: x => {
              return this.unitCallback(x.value)
            }
          }
        },
        scales: {
          x: {
            gridLines: {
              display: false
            }
          },
          y: {
            ticks: {
              callback: this.unitCallback
            },
            min: this.min,
            max: this.max
          }
        }
      }
    }
  },
  methods: {
    metricLabel (metric) {
      const map = {
        cpu: 'CPU',
        hdd_read: 'Disk Read',
        hdd_write: 'Disk Write',
        net_recv: 'Download',
        net_sent: 'Upload',
        ram: 'Memory'
      }
      return map[metric]
    }
  }
}
</script>