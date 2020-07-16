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
    },
    batch: Boolean
  },
  data: () => ({
    loaded: false,
    chartdata: null,
  }),
  computed: {
    options () {
      return {
        legend: {
          display: false
        },
        scales: {
          y: {
            type: this.batch ? 'logarithmic' : 'linear',
            ticks: {
              autoSkipPadding: 15
            }
          }
        }
      }
    }
  },
  methods: {
    loadData () {
      const endpoint = this.batch ? 'batch' : 'job'
      let target = `${this.$serverAddr}/chart/${endpoint}Workunits`
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