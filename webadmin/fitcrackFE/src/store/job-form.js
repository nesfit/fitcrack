// Vuex module for the Add Job view

// Props that don't save to templates
const base = {
  step: 1, // Form stepper step
  attackSettingsTab: 0, // Selected attack mode
  // info
  name: '',
  // input
  inputMethod: 'multipleHashes',
  hashList: '',
  hashType: null,
  // other
  startDate: null, // init!
  endDate: null // init!
}

// Basic empty template
export const empty = {
  template: 'Empty',
  // info
  comment: '',
  // settings
  leftDicts: [],
  rightDicts: [],
  rules: null,
  ruleLeft: null,
  ruleRight: null,
  masks: [],
  charset: [],
  submode: 2,
  markov: null, // init!
  markovThresh: NaN,
  pcfg: null,
  keyspaceLimit: 1321431161,
  // hosts
  hosts: [],
  // other
  startNow: true,
  endNever: true,
  timeForJob: 3600
}

export default {
  state: {
    ...base,
    ...empty
  },
  getters: {
    jobSettings (state) {
      return {}
    },
    valid (state) {
      return true
    }
  },
  mutations: {
    update (state, changes) {
      state = {
        ...state,
        ...changes
      }
    },
    apply (state, template = '{}') {
      state = {
        ...base,
        ...empty,
        ...JSON.parse(template)
      }
    }
  }
}