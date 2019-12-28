// Vuex module for the Add Job view
const mut = prop => (state, val) => { state[prop] = val }

function supermutator9000 (obj) {
  const muts = {}
  Object.keys(obj).forEach(key => {
    muts[`${key}Mut`] = mut(key)
  })
  return muts
}

// Props that don't save to templates
const base = {
  step: 1, // Form stepper step
  attackSettingsTab: undefined, // Selected attack mode
  validatedHashes: [],
  // info
  name: '',
  // input
  inputMethod: 'multipleHashes',
  hashList: '',
  hashType: null,
  // other
  startDate: null, // init!
  endDate: null // init!,
}

// Basic empty template
export const empty = {
  template: 'Empty',
  // info
  comment: '',
  // settings
  leftDicts: [],
  rightDicts: [],
  rules: [],
  ruleLeft: '',
  ruleRight: '',
  masks: [''],
  hybridMask: '',
  charset: [],
  submode: 2,
  markov: [], // init!
  markovThresh: NaN,
  pcfg: [],
  keyspaceLimit: 0,
  // hosts
  hosts: [],
  // other
  startNow: true,
  endNever: true,
  timeForJob: 3600
}

export default {
  namespaced: true,
  state: {
    ...base,
    ...empty
  },
  getters: {
    attackSettings (state) {
      const current = attacks.find(a => a.handler === state.attackSettingsTab)
      if (!current) return false
      return {
        'attack_mode': current.id,
        'attack_name': current.serverName,
        'rules': state.rules.length > 0 ? state.rules[0] : null,
        'left_dictionaries': state.leftDicts,
        'right_dictionaries': state.rightDicts,
        'rule_left': state.ruleLeft,
        'rule_right': state.ruleRight,
        'masks': state.masks,
        'attack_submode': state.submode,
        'markov_treshold': state.submode == 0 ? null : parseInt(state.markovThresh),
        'markov': state.submode == 0 ? null : state.markov[0],
        'charset': state.charset,
        'mask': state.hybridMask,
        'pcfg_grammar': state.pcfg[0],
        'keyspace_limit': state.keyspaceLimit
      }
    },
    jobSettings (state, { attackSettings }) {
      return {
        "name": state.name,
        "comment": state.comment,
        "priority": 0,
        "hosts_ids": state.hosts.map(h => h.id),
        "seconds_per_job": parseInt(state.timeForJob),
        "time_start": (state.startNow ? '' : state.startDate),
        "time_end": (state.endNever ? '' : state.endDate),
        'attack_settings': attackSettings,
        "hash_settings": {
          "hash_type": state.hashType ? state.hashType.code : null,
          "hash_list": state.validatedHashes
        }
      }
    },
    valid (state, { attackSettings }) {
      return (
        state.attackSettingsTab &&
        state.hosts.length > 0 &&
        attackSettings &&
        state.hashType !== null &&
        state.name !== ''
      )
    }
  },
  mutations: {
    ...supermutator9000({
      ...base,
      ...empty
    }),
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
    },
    //
    addMask (state, val = '') {
      state.masks.push(val)
    },
    deleteMask (state, index) {
      if (state.masks.length <= 1) return
      state.masks.splice(index, 1)
    },
    updateMask (state, {index, val}) {
      state.masks[index] = val
    },
    mergeMasks (state, arr) {
      if (state.masks[0] == '') state.masks.shift()
      state.masks = [...state.masks, ...arr]
    }
  }
}

export const attacks = [
  {handler: 'dictionary', name: 'Dictionary', id: 0, serverName: 'dict'},
  {handler: 'combinator', name: 'Combination', id: 1, serverName: 'combinator'},
  {handler: 'maskattack', name: 'Brute-force', id: 3, serverName: 'mask'},
  {handler: 'hybridWordlistMask', name: 'Hybrid Wordlist + Mask', id: 6, serverName: 'Hybrid wordlist+mask'},
  {handler: 'hybridMaskWordlist', name: 'Hybrid Mask + Wordlist', id: 7, serverName: 'Hybrid mask+wordlist'},
  {handler: 'pcfgAttack', name: 'PCFG', id: 9, serverName: 'pcfg'},
]