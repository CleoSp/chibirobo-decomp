#include "Version.h"

extern "C" {
#if defined(VERSION_JP)
extern const char sJapanesePCWeather[] = "天気予報の時間だＹＯ！　大型で非常に強い台風が近づいているＹＯ！　暴風や高波に厳重に警戒してＮＥ"
                                         "！　これからいよいよ秋とうらいＤＡＮＥ！　ピン・ポン・パン・ポン♪　　　　　　　　　　";
extern const char sJapanesePCSports[] = "スポーツの時間だＹＯ！　オリンピックも終わってみんな寝不足も解消されたかＮＡ！　"
                                        "でもまだまだ眠れないときがやってくるＹＯ！　オレンジ社たいこうちびロボピック！　"
                                        "各家庭のちびロボたちがいっせいに集まってお手伝いを競うんだＹＯ！　"
                                        "わたしの好きな競技はごみ箱フリースロー！　真剣にごみ箱を見つめるちびロボにぞっこんなＮＯ！　あと１"
                                        "週間で開催ＤＡＹＯ！　みんなお楽しみにＮＥ！　ピン・ポン・パン・ポン♪　　　　　　　　　　";
extern const char sJapanesePCHovercopter[] = "ちびロボを生みだしたオレンジ社がそうりょくをあげて　"
                                             "けんきゅうしてきた…ちびアタッチメントが　ついに完成！　\007ちびアタッチメントシリーズ　"
                                             "第一弾　「ホバコプター」が新はつばい！！　これさえあれば…\007ちょっとした　"
                                             "みぞを飛びこえることも！　\007つくえからゆかに　飛びおりることも！　"
                                             "\007そのさいだい飛きょりは　なんと１０ｃｍ！！　しかも！　"
                                             "５００マネとちょーおトクなプライス！！　\007ちびアタッチメントシリーズ　第一弾　「ホバコプタ"
                                             "ー」　うりきれ　すんぜん！！　　　　　　　　　　";
extern const char sJapanesePCRankingEncouragement[]
    = "あなたのランキングは　ただいま　\011位　となっています　もう少しがんばりましょう！　　　　　　　　　　";
extern const char sJapanesePCRankingPoor[]
    = "あなたのランキングは　ただいま　\011位　となっています　ぜんぜんダメです！　　　　　　　　　　";
extern const char sJapanesePCRankingKeepTrying[]
    = "あなたのランキングは　ただいま　\011位　となっています　あと少しがんばりましょう！　　　　　　　　　　";
extern const char sJapanesePCRankingGood[]
    = "あなたのランキングは　ただいま　\011位　となっています　なかなかがんばってますね！　　　　　　　　　　";
extern const char sJapanesePCRankingVeryGood[]
    = "あなたのランキングは　ただいま　\011位　となっています　すごくがんばってますね！　　　　　　　　　　";
extern const char sJapanesePCRankingGoalInSight[]
    = "あなたのランキングは　ただいま　\011位　となっています　ゴールがみえてきましたね！　　　　　　　　　　";
extern const char sJapanesePCRankingAlmostThere[] = "あなたのランキングは　ただいま　\011位　となっています　ここまでくればあとわずかです　"
                                                    "このいきおいでがんばりましょう！　　　　　　　　　　";
extern const char sJapanesePCRankingExcitement[]
    = "あなたのランキングは　ただいま　\011位　となっています　なんだか久しぶりに興奮してきました！　　　　　　　　　　";
extern const char sJapanesePCRankingNearTop[]
    = "あなたのランキングは　ただいま　\011位　となっています　天井がみえてきましたね！　　　　　　　　　　";
extern const char sJapanesePCRankingFirst[]
    = "あなたのランキングは　ただいま　\011位　となっています　やりました　１位　達成です！　　　　　　　　　　";
extern const char sJapanesePCRankingBest[]
    = "あなたのランキングは　ただいま　\011位　となっています　もうこれ以上あがらなくらいサイコーです！　　　　　　　　　　";
extern const char sJapanesePCShopping[] = "たのしい　たのしい　おかいもの♪　きょうは何を買おうかしら？　　　　　　　　　　";
extern const char sJapanesePCSpendMoney[] = "お金を払って世の中をうまく渡っちゃおう！　　　　　　　　　　";
extern const char sJapanesePCBuildUtilibots[] = "ジャンクを集めてレチッカーズを作ろう！　これぞリサイクル道！　　　　　　　　　　";
extern const char sJapanesePCExchangeScrap[] = "あまったジャンクをマネにかえて　かしこく　くらそう！　　　　　　　　　　";
extern const char sJapanesePCChibiPC[] = "ちびパソです　　　　　　　　　　";
extern const char sJapanesePCWelcome[]
    = "こんにちは　アクセスありがとうございます　オレンジ社がほこるハイエンドマシン　"
      "ちびパソです　どんな商品もまたたく間に届く　オンラインショップはいかがですか？　"
      "価格は常時２０％オフ！　充実の品ぞろえが　あなたをお待ちしております　"
      "地球にやさしいエコロジー派は　ジャンクマシーンをどうぞ　どんなジャンクでも　"
      "オレンジ社が回収します　さぁ　今日は何をつくりますか？　まずは　ご希望のメニューからどうぞ・・・　　　　　　　　　　　";
const char* gJapanesePCDialogue[] = {
	sJapanesePCWeather,
	sJapanesePCSports,
	sJapanesePCHovercopter,
	sJapanesePCRankingEncouragement,
	sJapanesePCRankingPoor,
	sJapanesePCRankingEncouragement,
	sJapanesePCRankingKeepTrying,
	sJapanesePCRankingGood,
	sJapanesePCRankingVeryGood,
	sJapanesePCRankingGoalInSight,
	sJapanesePCRankingAlmostThere,
	sJapanesePCRankingExcitement,
	sJapanesePCRankingNearTop,
	sJapanesePCRankingFirst,
	sJapanesePCRankingBest,
	sJapanesePCShopping,
	sJapanesePCSpendMoney,
	sJapanesePCBuildUtilibots,
	sJapanesePCExchangeScrap,
	sJapanesePCChibiPC,
	sJapanesePCWelcome,
	sJapanesePCWelcome,
	sJapanesePCWelcome,
	sJapanesePCWelcome,
};
#else
extern const char sEnglishPCWeather[]        = "And now, for the weather. A powerful typhoon is approaching, so watch out for strong wind "
                                               "and HUGE waves! And remember, autumn is just around the corner!           ";
extern const char sEnglishPCSports[]         = "And now, for sports! I hope you've rested up, because the Chibilympics are just around "
                                               "the corner! That's right! Citrusoft presents the single greatest test of robo-athletics "
                                               "known to man! It's time for every Chibi-Robo everywhere to come together and see who's "
                                               "the best helper! You know what I'm looking forward to? The trash-can free throws! I can't "
                                               "get enough of those little guys! The competition begins in one week! Don't miss out! Back "
                                               "to you, Ron!           ";
extern const char sEnglishPCBlank[]          = "           ";
extern const char sEnglishPCShopping[]       = "Shopping time, shopping time! What will I buy today?           ";
extern const char sEnglishPCBuildUtilibots[] = "Collect Scrap to build Utilibots!           ";
extern const char sEnglishPCExchangeScrap[]  = "Swap your spare Scrap for Moolah and help build a better world!           ";
extern const char sEnglishPCWelcome[]        = "Hello, and thank you for accessing the Chibi-PC. Citrusoft Robotics is proud to present "
                                               "you with this unique shopping experience: a home PC that delivers you any item within "
                                               "seconds of ordering! A wide world of shopping awaits you. And if you're eco-friendly, "
                                               "you'll love the Recyclotron, where your detritus is our delight! So, what do you want to "
                                               "make today? Please select from the menu.           ";
const char* gEnglishPCDialogue[]             = {
    sEnglishPCWeather,       sEnglishPCSports, sEnglishPCBlank,   sEnglishPCBlank,    sEnglishPCBlank, sEnglishPCBlank,
    sEnglishPCBlank,         sEnglishPCBlank,  sEnglishPCBlank,   sEnglishPCBlank,    sEnglishPCBlank, sEnglishPCBlank,
    sEnglishPCBlank,         sEnglishPCBlank,  sEnglishPCBlank,   sEnglishPCShopping, sEnglishPCBlank, sEnglishPCBuildUtilibots,
    sEnglishPCExchangeScrap, sEnglishPCBlank,  sEnglishPCWelcome,
};
#endif
}
