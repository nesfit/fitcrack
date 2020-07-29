<template>
  <div class="container">
    <timeseries
      v-if="loaded"
      :chart-data="chartdata"
      :overrides="options"
    />
  </div>
</template>

<script>
import Timeseries from './types/timeseries'
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
    from: {
      type: String,
      default: undefined
    },
    to: {
      type: String,
      default: undefined
    },
    batch: Boolean,
    logarithmic: Boolean
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
            type: this.batch || this.logarithmic ? 'logarithmic' : 'linear',
            ticks: {
              autoSkipPadding: 15
            }
          }
        }
      }
    },
    fromTime () {
      if (!this.id && !this.from) {
        return this.$moment().subtract(24, 'hours').format('YYYY-M-DTH:mm:ss')
      } else {
        return this.from
      }
    }
  },
  methods: {
    loadData () {
      const endpoint = this.batch ? 'batch' : 'job'
      let target = `${this.$serverAddr}/chart/${endpoint}Workunits`
      if (this.id) target += `/${this.id}`
      this.axios.get(target, {
        params: {
          from: this.fromTime,
          to: this.to
        }
      }).then(r => {
        this.chartdata = {
          datasets: prepareLines(r.data.datasets)
        }
        this.loaded = true
      })
    }
  }
}
</script>