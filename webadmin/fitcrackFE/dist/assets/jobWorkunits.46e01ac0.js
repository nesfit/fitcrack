import{T as s,l as o,p as n}from"./timeseries.9158092b.js";import{n as l}from"./index.9dfa3add.js";var d={props:{updateInterval:{type:Number,default:5e3}},data(){return{_interval:null}},mounted(){this.loadData(),this.updateInterval>0&&(this._interval=setInterval(this.loadData,this.updateInterval))},beforeDestroy(){clearInterval(this._interval)}},u={data(){return{_timeout:null}},watch:{from(){this.triggerLoad()},to(){this.triggerLoad()}},methods:{triggerLoad(){this.loading=!0,clearTimeout(this._timeout),this._timeout=setTimeout(()=>{this.loadData(!0)},100)}}},h=function(){var t=this,e=t.$createElement,a=t._self._c||e;return a("div",{staticClass:"container"},[a("v-progress-linear",{directives:[{name:"show",rawName:"v-show",value:t.loading,expression:"loading"}],attrs:{indeterminate:"",absolute:""}}),t.loaded?a("timeseries",{staticClass:"chart-wrapper",attrs:{"chart-data":t.chartdata,overrides:t.options}}):t._e()],1)},m=[];const c={components:{Timeseries:s},mixins:[d,u],props:{id:{type:Number,default:void 0},from:{type:String,default:void 0},to:{type:String,default:void 0},batch:Boolean,logarithmic:Boolean},data:()=>({loaded:!1,timeout:null,loading:!1,chartdata:null}),computed:{options(){return{plugins:{legend:{display:!1}},scales:{y:{type:this.batch||this.logarithmic?"logarithmic":"linear",ticks:{autoSkipPadding:15}}}}},fromTime(){return!this.id&&!this.from?this.$moment.utc().subtract(24,"hours").format("YYYY-M-DTH:mm:ss"):this.from}},methods:{loadData(t=!1){t&&(this.loading=!0);const e=this.batch?"batch":"job";let a=`${this.$serverAddr}/chart/${e}Workunits`;this.id&&(a+=`/${this.id}`),this.axios.get(a,{params:{from:this.fromTime,to:this.to}}).then(r=>{this.chartdata={datasets:o(n(r.data.datasets))},this.loading=!1,this.loaded=!0})}}},i={};var p=l(c,h,m,!1,f,"0868cc0c",null,null);function f(t){for(let e in i)this[e]=i[e]}var g=function(){return p.exports}();export{d as a,u as b,g as w};