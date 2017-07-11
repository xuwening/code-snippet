import * as types from './actionTypes';

//所有 actions，将 type 组装为状态对象{type:xxx, ...}
//action 是个返回状态的函数



export function increment() {
  return {
    type: types.INCREMENT
  };
}

export function decrement() {
  return {
    type: types.DECREMENT
  };
}

export function incrementNum(n) {
    return {
        type: types.OTHERMENT,
        num: n
    }
}
