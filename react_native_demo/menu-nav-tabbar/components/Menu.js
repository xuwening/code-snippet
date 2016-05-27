const React = require('react-native');
const {
  Dimensions,
  StyleSheet,
  ScrollView,
  View,
  Image,
  Text,
  Component,
  TouchableOpacity,
} = React;

import NavigationBar from 'react-native-navbar';

const window = Dimensions.get('window');
const uri = 'http://pickaface.net/includes/themes/clean/img/slide2.png';

const styles = StyleSheet.create({
  menu: {
    flex: 1,
    width: window.width,
    height: window.height,
    backgroundColor: 'gray',
    padding: 20,
  },
  avatarContainer: {
    marginBottom: 20,
    marginTop: 20,
  },
  avatar: {
    width: 48,
    height: 48,
    borderRadius: 24,
    flex: 1,
  },
  name: {
    position: 'absolute',
    left: 70,
    top: 20,
  },
  item: {
    fontSize: 18,
    fontWeight: '300',
    paddingTop: 20,
  },
    containerSec: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    // backgroundColor: 'green',
  },
});


class SecondView extends Component {

  render() {

   const rightButtonConfig = {
      title: 'share',
      handler: () => alert('hello!'),
    };

    const leftButtonConfig = {
      title: 'back',
      handler: () => this.props.nav.pop(),
    };

    const titleConfig = {
      title: 'Second View',
    };

    return (
      <View style={{ flex: 1, }}>
        <NavigationBar
          title={titleConfig}
          leftButton={leftButtonConfig}
          rightButton={rightButtonConfig} />

      </View>
    );

  }
}

module.exports = class Menu extends Component {

  constructor(props) {
    super(props);
    this.state = {

    };
  }

  static propTypes = {
    onItemSelected: React.PropTypes.func.isRequired,
  };

  itemAction() {

    this.props.onItemSelected('Search');
    this.props.nav.push({});
  }

  render() {
    return (
      <ScrollView scrollsToTop={false} style={styles.menu}>
        <View style={styles.avatarContainer}>
          <Image
            style={styles.avatar}
            source={{ uri, }}/>
          <Text style={styles.name}>18291835755</Text>
        </View>

        <Text
          onPress={ () => this.props.nav.push({component: SecondView})}
          style={styles.item}>
          搜索
        </Text>

        <Text
          onPress={() => this.props.nav.push({component: SecondView})}
          style={styles.item}>
          扫一扫
        </Text>

        <Text
          onPress={() => this.props.nav.push({component: SecondView})}
          style={styles.item}>
          分享推荐
        </Text>

        <Text
          onPress={() => this.props.onItemSelected('Application')}
          style={styles.item}>
          应用广场
        </Text>

        <Text
          onPress={() => this.props.nav.push({component: SecondView})}
          style={styles.item}>
          关于我们
        </Text>

      </ScrollView>
    );
  }
};
