import Vue from 'vue'
import Vuex from 'vuex'

import jobForm from './job-form'

Vue.use(Vuex)

export default new Vuex.Store({
  modules: {
    jobForm
  }
})