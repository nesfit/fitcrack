import{T as r,p as i}from"./timeseries.08527a7f.js";import{a as o,b as n}from"./jobWorkunits.cdd9973e.js";import{n as d}from"./index.0b60da2f.js";var l=function(){var e=this,t=e.$createElement,s=e._self._c||t;return s("div",{staticClass:"container"},[s("v-progress-linear",{directives:[{name:"show",rawName:"v-show",value:e.loading,expression:"loading"}],attrs:{indeterminate:"",absolute:""}}),e.loaded?s("timeseries",{staticClass:"chart-wrapper",attrs:{"chart-data":e.chartdata,overrides:e.options}}):e._e()],1)},m=[];const c={components:{Timeseries:r},mixins:[o,n],props:{id:{type:Number,default:void 0},from:{type:String,default:void 0},to:{type:String,default:void 0}},data:()=>({loaded:!1,loading:!1,chartdata:null,options:{elements:{line:{fill:!0}},plugins:{legend:{display:!1}},scales:{x:{gridLines:{display:!1}},y:{ticks:{callback(e){return`${e} %`}},min:0,max:100}}}}),computed:{fromTime(){return!this.id&&!this.from?this.$moment().subtract(24,"hours").format("YYYY-M-DTH:mm:ss"):this.from}},methods:{loadData(e=!1){e&&(this.loading=!0);let t=`${this.$serverAddr}/chart/jobProgress`;this.id&&(t+=`/${this.id}`),this.axios.get(t,{params:{from:this.fromTime,to:this.to}}).then(s=>{this.chartdata={datasets:i(s.data.datasets)},this.loading=!1,this.loaded=!0})}}},a={};var u=d(c,l,m,!1,f,"2d7c0532",null,null);function f(e){for(let t in a)this[t]=a[t]}var v=function(){return u.exports}();export{v as j};
