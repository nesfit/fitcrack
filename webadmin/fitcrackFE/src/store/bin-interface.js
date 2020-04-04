import { supermutator9000 } from '@/store'

import router from '@/router'
import axios from 'axios'
const api = window.serverAddress

// Vuex module for the Bins UI and job views communication

const state = {
  loading: false,
  bins: [],
  selectedJobs: []
}

export default {
  namespaced: true,
  state,
  getters: {
    
  },
  mutations: {
    ...supermutator9000(state),
    //
    loading (state) {
      state.loading = true
    },
    idle (state) {
      state.loading = false
    },
    unselect (state) {
      state.selectedJobs = []
    },
    //
    populate (state, arr) {
      state.bins = arr
    },
    rename (state, { id, name }) {
      state.bins.find(bin => bin.id == id).name = name
    }
  },
  actions: {
    async load ({ commit }) {
      commit('loading')
      const bins = await axios.get(`${api}/bins`).then(r => r.data.items)
      commit('idle')
      commit('populate', bins)
    },
    async create ({ commit, dispatch }, name) {
      commit('loading')
      await axios.post(`${api}/bins`, {name})
      dispatch('load')
    },
    move ({ commit, state }, { id, position, oldIndex }) {
      commit('loading')
      return axios.patch(`${api}/bins/${id}/move`, {position})
        .catch(e => {
          console.warn('Undoing move operation on bins')
          const arr = state.bins
          const bin = arr[position]
          arr.splice(position, 1)
          arr.splice(oldIndex, 0, bin)
          commit('populate', arr)
        })
        .finally(() => {
          commit('idle')
        })
    },
    async assign ({ commit }, { id, payload }) {
      commit('loading')
      await axios.put(`${api}/bins/${id}/assign`, payload)
      commit('idle')
      commit('unselect')
    },
    async rename ({ commit }, { id, newName }) {
      commit('loading')
      try {
        await axios.patch(`${api}/bins/${id}`, {name: newName})
        commit('rename', {id, name: newName})
      } catch (e) {
        console.error('Rename failed')
      } finally {
        commit('idle')
      }
    },
    async delete ({ commit, dispatch }, id) {
      commit('loading')
      await axios.delete(`${api}/bins/${id}`)
      router.replace({ name: 'jobs' })
      dispatch('load')
    }
  }
}