<<<<<<<< HEAD:webadmin/fitcrackFE/dist/assets/resourceMonitor.b2c3d728.js
import{T as s,p as r,a as n}from"./timeseries.9158092b.js";import{n as l}from"./index.d3f441ac.js";var m=function(){var t=this,e=t.$createElement,a=t._self._c||e;return a("div",{staticClass:"container"},[a("timeseries",{staticClass:"chart-wrapper",attrs:{"chart-data":t.chartdata,overrides:t.options}})],1)},o=[];const c={components:{Timeseries:s},props:{usageData:{type:Array,default:()=>[]},metrics:{type:Array,default:()=>[]},unitCallback:{type:Function,default:t=>t},min:{type:Number,default:0},max:{type:Number,default:void 0},rightScale:{type:Boolean},unitCallbackRight:{type:Function,default:t=>t},minRight:{type:Number,default:0},maxRight:{type:Number,default:void 0}},computed:{chartdata(){return{datasets:r(this.metrics.map(t=>({data:n(this.usageData,t),label:this.metricLabel(t)})))}},options(){return{elements:{line:{fill:!0},point:{radius:0}},plugins:{tooltip:{mode:"index",position:"nearest",callbacks:{label:t=>{const e=t.dataset.label;return`${t.formattedValue} ${this.labelUnit(e)}`}}}},scales:{x:{type:"time",time:{stepSize:5,tooltipFormat:"YYYY-MM-DD HH:mm",displayFormats:{millisecond:"HH:mm:ss.SSS",second:"HH:mm:ss",minute:"HH:mm",hour:"HH"}},grid:{display:!1}},y:{ticks:{callback:t=>{const e=this.metricLabel(this.metrics[0]);return`${t} ${this.labelUnit(e)}`}},min:this.min,max:this.max},y1:{position:"right",display:this.rightScale,ticks:{callback:t=>{const e=this.metricLabel(this.metrics[1]);return`${t} ${this.labelUnit(e)}`}},min:this.minRight,max:this.maxRight}}}}},methods:{metricLabel(t){return{cpu:"CPU",hdd_read:"Disk Read",hdd_write:"Disk Write",net_recv:"Download",net_sent:"Upload",ram:"Memory",speed:"Speed",temperature:"Temperature",utilization:"Utilization"}[t]},labelUnit(t){return{CPU:"%","Disk Read":"kb/s","Disk Write":"kb/s",Download:"kb/s",Upload:"kb/s",Memory:"%",Speed:"H/s",Temperature:"\xB0C",Utilization:"%"}[t]}}},i={};var p=l(c,m,o,!1,d,null,null,null);function d(t){for(let e in i)this[e]=i[e]}var b=function(){return p.exports}();export{b as R};
========
import{T as s,p as r,a as n}from"./timeseries.9158092b.js";import{n as l}from"./index.c11e1020.js";var m=function(){var t=this,e=t.$createElement,a=t._self._c||e;return a("div",{staticClass:"container"},[a("timeseries",{staticClass:"chart-wrapper",attrs:{"chart-data":t.chartdata,overrides:t.options}})],1)},o=[];const c={components:{Timeseries:s},props:{usageData:{type:Array,default:()=>[]},metrics:{type:Array,default:()=>[]},unitCallback:{type:Function,default:t=>t},min:{type:Number,default:0},max:{type:Number,default:void 0},rightScale:{type:Boolean},unitCallbackRight:{type:Function,default:t=>t},minRight:{type:Number,default:0},maxRight:{type:Number,default:void 0}},computed:{chartdata(){return{datasets:r(this.metrics.map(t=>({data:n(this.usageData,t),label:this.metricLabel(t)})))}},options(){return{elements:{line:{fill:!0},point:{radius:0}},plugins:{tooltip:{mode:"index",position:"nearest",callbacks:{label:t=>{const e=t.dataset.label;return`${t.formattedValue} ${this.labelUnit(e)}`}}}},scales:{x:{type:"time",time:{stepSize:5,tooltipFormat:"YYYY-MM-DD HH:mm",displayFormats:{millisecond:"HH:mm:ss.SSS",second:"HH:mm:ss",minute:"HH:mm",hour:"HH"}},grid:{display:!1}},y:{ticks:{callback:t=>{const e=this.metricLabel(this.metrics[0]);return`${t} ${this.labelUnit(e)}`}},min:this.min,max:this.max},y1:{position:"right",display:this.rightScale,ticks:{callback:t=>{const e=this.metricLabel(this.metrics[1]);return`${t} ${this.labelUnit(e)}`}},min:this.minRight,max:this.maxRight}}}}},methods:{metricLabel(t){return{cpu:"CPU",hdd_read:"Disk Read",hdd_write:"Disk Write",net_recv:"Download",net_sent:"Upload",ram:"Memory",speed:"Speed",temperature:"Temperature",utilization:"Utilization"}[t]},labelUnit(t){return{CPU:"%","Disk Read":"kb/s","Disk Write":"kb/s",Download:"kb/s",Upload:"kb/s",Memory:"%",Speed:"H/s",Temperature:"\xB0C",Utilization:"%"}[t]}}},i={};var p=l(c,m,o,!1,d,null,null,null);function d(t){for(let e in i)this[e]=i[e]}var b=function(){return p.exports}();export{b as R};
>>>>>>>> master:webadmin/fitcrackFE/dist/assets/resourceMonitor.476c838d.js
