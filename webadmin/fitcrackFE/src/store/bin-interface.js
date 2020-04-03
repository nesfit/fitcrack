import Vue from 'vue'
import { supermutator9000 } from '@/store'

// Vuex module for the Bins UI and job views communication

const state = {
  bins: [
    { id: 1, name: 'Examples' },
    { id: 2, name: 'Prince Tests' },
    { id: 3, name: 'My Collection' },
    //...(function(){let x=[];for(let i=4;i<20;i++){x.push({id:i,name:i.toString()})};return x})()
  ],
  selectedJobs: []
}

export default {
  namespaced: true,
  state,
  getters: {
    
  },
  mutations: {
    ...supermutator9000(state)
  }
}