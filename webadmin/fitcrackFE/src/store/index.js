import Vue from 'vue'
import Vuex from 'vuex'

import jobForm from './job-form'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    darkAppearance: false,
    project: window.projectName || 'Fitcrack',
    user: {
      'userData': {
        'id': null,
        'username': null,
        'mail': null,
        'role': {
          'MANAGE_USERS': true,
          'ADD_NEW_JOB': true,
          'UPLOAD_DICTIONARIES': true,
          'VIEW_ALL_JOBS': true,
          'EDIT_ALL_JOBS': true,
          'OPERATE_ALL_JOBS': true,
          'ADD_USER_PERMISSIONS_TO_JOB': true
        }
      },
      'loggedIn': false,
      'loggedInLink': null
    }
  },
  modules: {
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