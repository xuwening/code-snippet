

import React, {
  Component,
} from 'react';

import {

    AppRegistry,
    Image,
    StyleSheet,
    Text,
    View,
    TouchableOpacity,
    Navigator,
    TabBarIOS,
} from 'react-native';


class SecondView extends Component {

  pressAction() {
    this.props.nav.pop();
  }

  render() {


    return (
        <View style={styles.containerSec}>

          <TouchableOpacity onPress={this.pressAction.bind(this)}>
            <Text> pop back. </Text> 
          </TouchableOpacity>

        </View>
      );
  }
}


class HomePage extends Component {


  pressAction() {

      //每次调用push时，其实就是调用MyNavigator中的renderScene（_renderScene）
      this.props.nav.push({
          component: SecondView,
        });
  }

  render() {

    return (
        <View style={styles.container}>
          <TouchableOpacity onPress={this.pressAction.bind(this)}>
            <Text> this is a text. </Text> 
          </TouchableOpacity>
        </View>
      );
  }
}


class ReadDemo extends Component {

  constructor(props) {
    super(props);

    this.state = {
      
        selectedTab: 'blueTab',
        notifCount: 0,
        presses: 0,
    };

    this.statics = {
      title: '<TabBarIOS>',
      description: 'Tab-based navigation.',
    };

    this.displayName = 'TabBarExample';
  }

  _renderContent(color: string, pageText: string, num?: number) {
    return (
      <HomePage nav={this.props.nav} color={color}/>
    );
  }

  render() {

    return (

        <TabBarIOS
          tintColor="dodgerblue"
          barTintColor="white">

          <TabBarIOS.Item
            title="首页"
            icon={require('./png/home.png')}
            selected={this.state.selectedTab === 'blueTab'}
            onPress={() => {
              this.setState({
                selectedTab: 'blueTab',
              });
            }}>
            {this._renderContent('#414A8C', 'Blue Tab')}
          </TabBarIOS.Item>

          <TabBarIOS.Item
          title="我的"
          icon={require('./png/user.png')}
          badge={this.state.notifCount > 0 ? this.state.notifCount : undefined}
          selected={this.state.selectedTab === 'redTab'}
          onPress={() => {
            this.setState({
              selectedTab: 'redTab',
              notifCount: this.state.notifCount,
            });
          }}>
          {this._renderContent('#783E33', 'Red Tab', this.state.notifCount)}
          </TabBarIOS.Item>

          <TabBarIOS.Item
          icon={require('./png/business.png')}
          title="商城"
          badge={this.state.notifCount > 0 ? this.state.notifCount : undefined}
          selected={this.state.selectedTab === 'yellowTab'}
          onPress={() => {
            this.setState({
              selectedTab: 'yellowTab',
              notifCount: this.state.notifCount,
            });
          }}>
          {this._renderContent('#783E00', 'yellow Tab', this.state.notifCount)}
          </TabBarIOS.Item>

          <TabBarIOS.Item
          icon={require('./png/service.png')}
          title="服务"
          badge={this.state.notifCount > 0 ? this.state.notifCount : undefined}
          selected={this.state.selectedTab === 'brownTab'}
          onPress={() => {
            this.setState({
              selectedTab: 'brownTab',
              notifCount: this.state.notifCount,
            });
          }}>
          {this._renderContent('#780033', 'brown Tab', this.state.notifCount)}
          </TabBarIOS.Item>

          <TabBarIOS.Item
            icon={require('./png/score.png')}
            title="积分"
            selected={this.state.selectedTab === 'greenTab'}
            onPress={() => {
              this.setState({
                selectedTab: 'greenTab',
                presses: this.state.presses,
              });
            }}>
            {this._renderContent('#21551C', 'Green Tab', this.state.presses)}
          </TabBarIOS.Item>

        </TabBarIOS>

      );
  }
}


class MyNavigator extends Component {

  //跳转场景（全局）
  _renderScene(router, navigator) {

    var SomeComponent = router.component;
    return <SomeComponent nav={navigator} />
  }

  render() {

    return ( 

    <Navigator

      initialRoute={{component: ReadDemo}}

      configureScene={(route) => { return Navigator.SceneConfigs.HorizontalSwipeJump; }}

      renderScene={this._renderScene}/>

      );
  }
}


var styles = StyleSheet.create({

  container: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: 'white',
  },

  containerSec: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: 'green',
  },

  tabText: {
    width: 53,
    height: 81,
  },
});

AppRegistry.registerComponent('reactDemo', () => MyNavigator);

