

import React, {
  Component,
} from 'react';

import {

    AppRegistry,
    Navigator,
} from 'react-native';


const TabFrame = require('./components/TabFrameWork');


class MyNavigator extends Component {

  //跳转场景（全局）
  _renderScene(router, navigator) {

    var SomeComponent = router.component;
    var args = router.args;
    if (args) {
        return <SomeComponent nav={navigator} args={args}/>;      
    };

    return <SomeComponent nav={navigator} />;
  }

  render() {

    return ( 

    <Navigator

      initialRoute={{component: TabFrame}}

      configureScene={(route) => { return Navigator.SceneConfigs.HorizontalSwipeJump; }}

      renderScene={this._renderScene}/>

      );
  }
}

AppRegistry.registerComponent('reactDemo', () => MyNavigator);

