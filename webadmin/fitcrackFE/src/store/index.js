import Vue from 'vue'
import Vuex from 'vuex'

import user from './user'
import jobForm from './job-form'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    darkAppearance: false,
    project: window.projectName || 'Fitcrack'
  },
  modules: {
    user,
    jobForm
  }
})

export function twoWayMap (props) {
  const map = {}
  props.forEach(p => {
    map[p] = `${p}Mut`
  })
  return map
}