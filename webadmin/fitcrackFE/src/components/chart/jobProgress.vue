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
    }
  },
  data: () => ({
    loaded: false,
    chartdata: null,
    options: {
      elements: {
        line: {
          fill: true
        }
      },
      legend: {
        display: false
      },
      scales: {
        x: {
          gridLines: {
            display: false
          }
        },
        y: {
          ticks: {
            callback (val) {
              return `${val} %`
            }
          },
          min: 0,
          max: 100
        }
      }
    }
  }),
  computed: {
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
      let target = `${this.$serverAddr}/chart/jobProgress`
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