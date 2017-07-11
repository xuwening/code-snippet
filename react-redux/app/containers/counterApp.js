'use strict';

import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import Counter from '../components/counter';
import * as counterActions from '../actions/counterActions';
import { connect } from 'react-redux';

// @connect(state => ({
//   state: state.counter
// }))
class CounterApp extends Component {

  render() {
    const { state, actions } = this.props;
    return (

        //counter actions 作为 props 传递给子组件
        // state作为一个整树，每个子组件，接收一个子 state
      <Counter
        counter={state.count}
        {...actions} />
    );
  }
}

//指定当前组件对应的 state，以及绑定当前组件需要分发的 action
export default connect(state => ({
    state: state.counter
  }),
  (dispatch) => ({
    actions: bindActionCreators(counterActions, dispatch)
  })
)(CounterApp);
