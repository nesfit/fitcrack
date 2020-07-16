<template>
  <div class="container">
    <timeseries
      v-if="loaded"
      :chart-data="chartdata"
      :options="options"
    />
  </div>
</template>

<script>
import Timeseries from './timeseries'
import { prepareLines } from './helpers'
import autoload from './autoupdateMixin'

export default {
  components: { Timeseries },
  mixins: [autoload],
  props: {
    id: {
      type: Number,
      default: undefined
    }
  },
  data: () => ({
    loaded: false,
    chartdata: null,
    options: {
      elements: {
        line: {
          fill: true,
          tension: 0
        }
      },
      legend: {
        display: false
      },
      scales: {
        x: {
          type: "time",
          gridLines: {
            display: false
          }
        },
        y: {
          ticks: {
            callback (val) {
              return `${val} %`
            }
          }
        }
      }
    }
  }),
  methods: {
    loadData () {
      let target = `${this.$serverAddr}/chart/jobProgress`
      if (this.id) target += `/${this.id}`
      this.axios.get(target).then(r => {
        this.chartdata = {
          datasets: prepareLines(r.data.datasets)
        }
        this.loaded = true
      })
    }
  }
}
</script>