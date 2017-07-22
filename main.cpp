
#include <QApplication>
#include <QTextCodec>
#include <iostream>
#include <string>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<map>
#include<fstream>
#include<algorithm>
#include "base.h"
#include <functional>
#include<cmath>
#include "change_yourself.h"
#include "menu_admin.h"
#include "menu_user.h"
#include "signin.h"
#include "ui_signin.h"
#include <QDialog>
#include "signup.h"
#include <QMessageBox>
#include "ui_signup.h"
#include "ui_menu_user.h"
#include "ui_change_yourself.h"
#include "ui_menu_admin.h"
#include "book_change.h"
#include "book_add.h"
#include "ui_book_add.h"
#include "ui_book_change.h"
#include <QInputDialog>
#include "adduser.h"
#include "ui_adduser.h"
#include "user_change.h"
#include "ui_user_change.h"
#include <QDir>
#include<QDebug>
#include<QCloseEvent>
using namespace std;
map<string, Book>books;                 //存书的map
map<string, User>users;                 //存用户的map
int maxBook, maxUser;                   //书数量的最大值、用户数最大值
int position=0;
map<string,string>Administrators;
string userid;
//string bookid;
vector<string> bookrecommendlist;
vector<string> userrecommendlist;
//long step=0;
//Loading *l0;
void Book::deletebook() {//删除图书
    map<string, int>::iterator iter = usersRead.begin();
    for (; iter != usersRead.end(); iter++)
    {
        users[iter->first].booksRead.erase(id);     //每个用户对此书的阅读 删掉
        users[iter->first].sum -= iter->second;        //该用户总分--
        users[iter->first].cnt--;                      //该用户评论数--
        users[iter->first].aveRank = users[iter->first].sum / users[iter->first].cnt;  //该用户平均分更新
    }
    cout << title << " has been deleted;" << endl;
    books.erase(id);                           //删除掉这本书
}
void Book::addbook() {
    books.insert(map<string, Book>::value_type(id, *this));
}
void Book::rechangebook(string s1, string s2) { //给书进行赋值
    if (s1 == "title:") {
        title = s2;
    }
    else if (s1 == "id:") {
        id = s2;
    }
    else if (s1 == "author:") {
        author = s2;
    }
    else if (s1 == "publisher:") {
        publisher = s2;
    }
    else if (s1 == "year:") {
        year = atoi(s2.c_str());
    }
    else if (s1 == "urlS:") {
        urlS = s2;
    }
    else if (s1 == "urlM") {
        urlM = s2;
    }
    else if (s1 == "urlL") {
        urlL = s2;
    }
}
void User::deleteuser() {           //删除用户
    map<string, int>::iterator iter = booksRead.begin();
    for (; iter != booksRead.end(); iter++)
    {
        books[iter->first].usersRead.erase(himself);
        books[iter->first].sum -= iter->second;
        books[iter->first].cnt--;
        books[iter->first].aveRank = books[iter->first].sum / books[iter->first].cnt;
    }
    cout << himself << " has been deleted;" << endl;
    users.erase(himself);
}
void User::addusers() {     //增添用户
    users.insert(map<string, User>::value_type(himself, *this));
}
void User::rechangeuser(string s1, string s2) {   //给用户赋值
    if (s1 == "town:") {
        town = s2;
    }
    else if (s1 == "city:") {
        city = s2;
    }
    else if (s1 == "country:") {
        country = s2;
    }
    else if (s1 == "age:") {
        age = atoi(s2.c_str());
    }
    else if (s1 == "name:") {
        himself = s2;
    }
}
//part3新增
Book User::findByName(string ti) {
    map<string, Book>::iterator it = books.begin();
    Book bk;
    int ind = 0;
    for (; it != books.end(); it++) {
        if (it->second.title == ti) {
            bk = it->second;
            ind = 1;
            break;
        }
    }
    if (ind == 1)
        return bk;
    else
        return books.end()->second;
}

void User::rank(int rank, string str) {
    //Book bk;
//用ISBN查找
    books[str].userRanked.insert(map<string, int>::value_type(himself, rank));
    booksRead.insert(map<string, int>::value_type(str, rank));
    books[str].cnt++;
//	cout<<books[str].cnt<<endl;
    books[str].sum += rank;
//	cout<<books[str].sum<<endl;
    books[str].aveRank = books[str].sum / books[str].cnt;
//	cout<<books[str].aveRank<<endl;
    sum += rank;
//	cout<<"sumnowis"<<sum<<endl;
    cnt += 1;
//	cout<<"cntnowis"<<cnt<<endl;
    aveRank = sum / cnt;
//	cout<<"averanknowis"<<aveRank<<endl;
    }

void User::add_comment(Book &b, string comm){
    b.comment.push_back(comm);
    b.commentauthor.push_back(this->himself);
}

void User::get_init_reco() {//获得初始推荐的书籍
    multimap<double, string>dist_map;
    map<string, User>::iterator user_iter = users.begin();
    map<string, Book>::iterator book_iter = books.begin();
    multimap<double, string,greater<double> >pred_map;
    for (; user_iter != users.end(); ++user_iter) {
        //计算当前用户与已有用户（根据地区、年龄的）相似度并存入dist_map
        if(user_iter->second.himself!="")
        dist_map.insert(make_pair(get_sim(user_iter->second), user_iter->second.himself));
    }
    multimap<double, string>::iterator it = dist_map.begin();
    for (int i = 0; i<200; ++i) {
        //获取200 nearest neighbors
        string id=it->second;
        sim_user.insert(make_pair(id,users.find(id)->second));
        ++it;
    }

    for (; book_iter != books.end(); ++book_iter) {
        double predSum;//预测函数的分子
        double simSum;//预测函数的分母
        map<string,User>::iterator it3=sim_user.begin();
        predSum = 0;
        simSum = 0;
        for (;it3!=sim_user.end();++it3) {
            if (it3->second.booksRead.find(book_iter->first)!=it3->second.booksRead.end()) {
                predSum += ((double)(it3->second.booksRead.find(book_iter->first)->second))
                    *(10-get_sim(it3->second));
                simSum += (10-get_sim(it3->second));
            }
        }
        if(simSum!=0){
            pred_map.insert(make_pair(predSum / simSum, book_iter->first));
        }
    }
    multimap<double, string>::iterator it2 = pred_map.begin();
    for (int i = 1; i <= 5 && it2 != pred_map.end(); ++it2) {
        if(books.find(it2->second)->second.title!=""){
        bookrecommendlist.push_back(books.find(it2->second)->first);
        i++;
    }
    }

    map<double,string>::iterator iter=dist_map.begin();
     for(int i=1;i<=5;++iter){
        if(iter->second!=himself) {
             userrecommendlist.push_back(users.find(iter->second)->first);
            i++;
         }
     }
}

double User::get_sim(User const& u) {//基于地区、年龄的相似度
    double sim1, sim2, sim;
    if (town == u.town) sim1 = 0;
    else if (city == u.city) sim1 = 5;
    else if (country == u.country) sim1 = 15;
    else sim1 = 30;
    sim2 = abs(age - u.age);
    sim = 0.2*sim1 + 0.8*sim2;
    return sim;

}
/*以下是基于用户的协同过滤算法*/
int cmp(const pair<string, double>& x, const pair<string, double>& y)
{
    return x.second > y.second;
}

void sortMapByValue(map<string, double>& tMap, vector<pair<string, double> >& tVector)
{
    for (map<string, double>::iterator curr = tMap.begin(); curr != tMap.end(); curr++)
        tVector.push_back(make_pair(curr->first, curr->second));

    sort(tVector.begin(), tVector.end(), cmp);
}
void User::getrecommendation() {
    //double*Sims = new double[maxUser];
    double sumup = 0, sumdown1 = 0, sumdown2 = 0, ssumup = 0, ssumdown = 0;  //最后要清空
    int cnt = 0;
    bool isequal=true;
    bool issimi=false;
    map<string, double>Sims;
    map<string, double> RankPre;
    vector<pair<string, double> > Rvector;
    vector<pair<string,double> > Svector;
    map<string, User>::iterator iter = users.begin();           //指向users的迭代器
    map<string, int>::iterator iter1 = this->booksRead.begin(); //指向booksRead的迭代器
    map<string, int>::iterator iter2;                           //指向booksRead的迭代器
    map<string, Book>::iterator iter3 = books.begin();         //指向books的迭代器
    map<string, int>::iterator iter4;                          //指向userRead的迭代器
    map<string, double>::iterator iter5;
    for (; iter != users.end(); iter++, cnt++) {//计算和每个其他用户的相似度
        if (himself == iter->first)continue;
        for (; iter1 != this->booksRead.end(); iter1++) {//找和此用户相同的读过的书
            iter2 = iter->second.booksRead.find(iter1->first);
            if (iter2 != iter->second.booksRead.end()) {//如果另一个用户读过这本书
        //	cout<<"in0"<<endl;
               issimi=true;
               if(iter1->second - this->aveRank!=iter2->second - iter->second.aveRank){
                isequal=false;
               }
                sumup += (iter1->second - this->aveRank)*(iter2->second - iter->second.aveRank);
                sumdown1 += (iter1->second - this->aveRank)*(iter1->second - this->aveRank);
                sumdown2 += (iter2->second - iter->second.aveRank)*(iter2->second - iter->second.aveRank);
            //	cout<<"sum:"<<sumup<<" "<<sumdown1<<" "<<sumdown2<<endl;
            }
        }
        iter1 = this->booksRead.begin();
        if(isequal&&issimi){ //如果一模一样的两个人，直接置1
        //	cout<<"in"<<endl;
            Sims.insert(map<string, double>::value_type(iter->first, 1));
        }
        else if (sumdown1&&sumdown2) { //如果他们有一块读过的书，否则无法计算
         //   cout<<"in2"<<endl;
            sumdown1 = sqrt(sumdown1);
            sumdown2 = sqrt(sumdown2);
            Sims.insert(map<string, double>::value_type(iter->first, sumup / (sumdown1*sumdown2))); //计算出相似度
        }
        //其他情况的话认为其评论数据不具辨识性，直接初等推荐.
        sumup = 0; sumdown1 = 0; sumdown2 = 0;isequal=true;issimi=false;
    }
    cout<<"in3"<<endl;
    if(!Sims.empty()){
        for (; iter3 != books.end(); iter3++) {//去算所有没读过的书的预测评分
        if (this->booksRead.find(iter3->first) == this->booksRead.end()) {//这本书这个人没读过，可以被推荐
            iter4 = iter3->second.usersRead.begin();
            for (; iter4 != iter3->second.usersRead.end(); iter4++) {//从读过这本书的人里去找
                iter5 = Sims.find(iter4->first);
                if (iter5 != Sims.end()) {//这个读者与被推荐用户有相似度
                    ssumup += iter5->second*(iter4->second - users.find(iter4->first)->second.aveRank);
                    ssumdown += iter5->second;
                }
            }
            if (ssumdown != 0)
                RankPre.insert(map<string, double>::value_type(iter3->first, this->aveRank + ssumup / ssumdown));
            else RankPre.insert(map<string, double>::value_type(iter3->first, 5));       //没有预测结果默认5分
        }
        ssumup = 0; ssumdown = 0;
    }
    //下面对RankPre进行排序后输出
    sortMapByValue(RankPre, Rvector);
    sortMapByValue(Sims, Svector);
    //cout << "You may like:" << endl;
    //cout<<"so we'll recommend you these books:"<<endl;
    for (int i = 0; bookrecommendlist.size()<=5;i++) {
        if(i>=Rvector.size())break;
        if(books.find(Rvector[i].first)->second.title!="")
          bookrecommendlist.push_back(books.find(Rvector[i].first)->first);
       // books.find(Rvector[i].first)->second.show();
    }
    //cout<<"and we'll recommend you these friends:"<<endl;
    //cout<<"aa"<<endl;
    for (int i = 0; i <5; i++) {
        if(i>=Svector.size())break;
        userrecommendlist.push_back(users.find(Svector[i].first)->first);
            users.find(Svector[i].first)->second.show();
    }
    //cout<<"aa"<<endl;

}
    else{
        //cout<<"but nobody is similar with you, so we'll recommend you these books:"<<endl;
    //	cout<<"he is unknowlegable"<<endl;
        this->get_init_reco();
    }
}
void readFile()
{
    QString runPath = QCoreApplication::applicationDirPath();
    runPath.append("/BX-Books.csv");
    string IS, ID, str, value;
    //ifstream file("F:\\Qt_book2\\BX-Books.csv");
    //cout<<runPath.toStdString();
    ifstream file(runPath.toStdString());
    if (file.peek() == EOF)
    {
        cout << "BX-Books.csv is empty." << endl;
        return;
    }
    getline(file, value, ';');
    getline(file, value, ';');
    getline(file, value, ';');
    getline(file, value, ';');
    getline(file, value, ';');
    getline(file, value, ';');
    getline(file, value, ';');
    getline(file, value, '\n');

    while (!file.eof())
    {
        Book newbook;

        getline(file, value, ';');
        IS = string(value, 1, value.length() - 2);
        newbook.id=IS;
        getline(file, value, ';');
        newbook.title = string(value, 1, value.length() - 2);

        getline(file, value, ';');
        newbook.author = string(value, 1, value.length() - 2);

        getline(file, value, ';');
        str = string(value, 1, value.length() - 2);
        newbook.year = str;

        getline(file, value, ';');
        newbook.publisher = string(value, 1, value.length() - 2);

        getline(file, value, ';');
        newbook.urlS = string(value, 1, value.length() - 2);

        getline(file, value, ';');
        newbook.urlM = string(value, 1, value.length() - 2);

        getline(file, value, '\n');
        newbook.urlL = string(value, 1, value.length() - 2);

        books.insert(map<string, Book>::value_type(IS, newbook));
    }
    file.close();
    maxBook = atoi(IS.c_str());
QString runPath2 = QCoreApplication::applicationDirPath();
    runPath2.append("/BX-Users.csv");

    ifstream file2(runPath2.toStdString());
    if (file2.peek() == EOF)
    {
        cout << "BX-Users.csv is empty." << endl;
        return;
    }
    getline(file2, value, ';');
    getline(file2, value, ';');
    getline(file2, value, '\n');
    while (!file2.eof())
    {
        User newUser;

        getline(file2, value, ';');
        ID = string(value, 1, value.length() - 2);
        newUser.himself = ID;
        getline(file2, value, ',');
        newUser.town = string(value, 1, value.length() - 1);
        getline(file2, value, ',');
        newUser.city = string(value, 1, value.length() - 1);
        getline(file2, value, ';');
        newUser.country = string(value, 1, value.length() - 2);

        getline(file2, value, ';');
        str = string(value, 1, value.length() - 2);
        newUser.age = atoi(str.c_str());

        getline(file2, value, '\n');
        newUser.password=string(value, 1, value.length() - 2);



        users.insert(map<string, User>::value_type(ID, newUser));
    }
    maxUser = atoi(ID.c_str());
    file2.close();
}

void writeback()
{
   QString runPath = QCoreApplication::applicationDirPath();
    runPath.append("/BX-Books.csv");
    //ofstream file("bxbook.csv");
    ofstream file(runPath.toStdString());
    //ofstream file("F:\\Qt_book2\\BX-Books.csv");
    file << "\"ISBN\";\"Book-Title\";\"Book-Author\";\"Year-Of-Publication\";\"Publisher\""
        << ";\"Image-URL-S\";\"Image-URL-M\";\"Image-URL-L\"" << endl;
    map<string, Book>::iterator iter = ++books.begin();
    for (; iter != books.end(); iter++)
    {
        if(iter->second.title=="")continue;
        file << "\"" << iter->first << "\";\"" << iter->second.title
            << "\";\"" << iter->second.author << "\";\"" << iter->second.year
            << "\";\"" << iter->second.publisher << "\";\"" << iter->second.urlS
            << "\";\"" << iter->second.urlM << "\";\"" << iter->second.urlL << "\"" << endl;
    }
    file << endl;
    file.close();

    QString runPath2 = QCoreApplication::applicationDirPath();
    runPath2.append("/BX-Users.csv");

    ofstream file2(runPath2.toStdString());
    file2 << "\"User-ID\";\"Location\";\"Age\";\"Password\"" << endl;
    map<string, User>::iterator iter2 = ++users.begin();
    for (; iter2 != users.end(); iter2++)
    {
        file2 << "\"" << iter2->first << "\";\"" << iter2->second.town
            << ", " << iter2->second.city << ", " << iter2->second.country << "\";";
        if(!iter2->second.age)
            file2 << "NULL" ;
        else
            file2 << "\"" << iter2->second.age << "\"" ;
        file2<<";\""<<iter2->second.password<<"\""<<endl;
    }
    file2 << endl;
    file2.close();

    QString runPath3 = QCoreApplication::applicationDirPath();
   runPath3.append("/BX-Book-Ratings.csv");
    ofstream file3(runPath3.toStdString());
    file3 << "\"User-ID\";\"ISBN\";\"Book-Rating\";\"User-Id" << endl;
    map<string, User>::iterator iter3 = ++users.begin();
    for (; iter3 != users.end(); iter3++)
    {
        if(iter3->second.booksRead.empty())
            continue;
        else
        {
            map<string, int>::iterator tmp = iter3->second.booksRead.begin();
            for(; tmp != iter3->second.booksRead.end(); tmp++)
                file3 << "\"" << iter3->first << "\";\"" << tmp->first
                    << "\";\"" << tmp->second << "\"" << endl;
        }
    }
    file3 << endl;
    file3.close();
}


void Rank()
{
    string IS, ID, str, value;
    //ifstream file("F:\\Qt_book2\\BX-Book-Ratings.csv");
    int rank;
    QString runPath2 = QCoreApplication::applicationDirPath();
    runPath2.append("/BX-Book-Ratings.csv");

    ifstream file(runPath2.toStdString());
    if (file.peek() == EOF)
    {
        cout << "BX-Book-Ratings.csv is empty." << endl;
        return;
    }
    getline(file, value, ';');
    getline(file, value, ';');
    getline(file, value, '\n');
    while (!file.eof())
    {
        getline(file, value, ';');
        ID = string(value, 1, value.length() - 2);
        users[ID].cnt++;

        getline(file, value, ';');
        IS = string(value, 1, value.length() - 2);
        books[IS].cnt++;

        getline(file, value, '\n');
        str = string(value, 1, value.length() - 2);
        rank = atoi(str.c_str());
        books[IS].sum += rank;
        users[ID].sum += rank;
        users[ID].booksRead.insert(map<string, int>::value_type(IS, rank));
        books[IS].usersRead.insert(map<string, int>::value_type(ID, rank));
    }
    file.close();
    map<string, Book>::iterator iter = books.begin();
    for (; iter != books.end(); iter++)
    {
        iter->second.aveRank = iter->second.sum / iter->second.cnt;
    }
    map<string, User>::iterator iter2 = users.begin();
    for (; iter2 != users.end(); iter2++)
    {
        iter2->second.aveRank = iter2->second.sum / iter2->second.cnt;
    }
}
/*以下是Qt的实现部分          */
void signin::on_pushButton_2_clicked()
{
    QString qstr,qpas;
    string str,pas;
    qstr=ui->lineEdit->text();
    qpas=ui->lineEdit_2->text();
    str=qstr.toStdString();
    pas=qpas.toStdString();
    map<string, User>::iterator iter;
    map<string,string>::iterator iter0=Administrators.begin();
    for(;iter0!=Administrators.end();iter0++){
        if(str==iter0->first&&pas==iter0->second){//进管理员菜单
            userid=str;
            position=2;
            close();
            menu_admin* menu_admin0=new menu_admin();
            menu_admin0->show();
            return ;
        }
    }
              //数据集奇少下有问题
        userid=str;
        iter = users.find(str);
        if (iter != users.end()&&pas==iter->second.password) {
            close();
            menu_user* menu_user0=new menu_user();
            menu_user0->show();
            if (iter->second.booksRead.empty()) {

                iter->second.get_init_reco();
            }
            else
            {
                iter->second.getrecommendation();
            }
            cout<<"b"<<endl;
            string name;
            menu_user0->ui->textBrowser->append("we'll recommend you these books:");
            for(int i=0;i<bookrecommendlist.size();i++){
                name=bookrecommendlist[i];
                QString qnam = QString::fromStdString(books.find(name)->second.id);
                menu_user0->ui->textBrowser->append(qnam);
                QString qname,qtitle,qauthor,qpublisher,qyear,qurlS, qurlM, qurlL,qaveRank,qsum;
                qname=QString::fromStdString(name);
                  qtitle=QString::fromStdString(books.find(name)->second.title);
                qauthor=QString::fromStdString(books.find(name)->second.author);

                qpublisher=QString::fromStdString(books.find(name)->second.publisher);
                qyear=QString::fromStdString(books.find(name)->second.year);
                qurlS=QString::fromStdString(books.find(name)->second.urlS);
                qurlM=QString::fromStdString(books.find(name)->second.urlM);
                qurlL=QString::fromStdString(books.find(name)->second.urlL);
                char ran[64];
                sprintf(ran, "%lf",books.find(name)->second.aveRank );
                string aveRANK(ran);
                qaveRank=QString::fromStdString(aveRANK.c_str());
                char rbn[64];
                sprintf(rbn, "%lf",books.find(name)->second.sum );
                string SUM(rbn);
                qsum=QString::fromStdString(SUM.c_str());
                menu_user0->ui->textBrowser->append("ISBN:");
               menu_user0-> ui->textBrowser->append(qname);
                menu_user0->ui->textBrowser->append("title:");
                menu_user0-> ui->textBrowser->append(qtitle);
                menu_user0->ui->textBrowser->append("author:");
                menu_user0->ui->textBrowser->append(qauthor);
                menu_user0->ui->textBrowser->append("publisher:");
                menu_user0->ui->textBrowser->append(qpublisher);
                menu_user0->ui->textBrowser->append("year:");
                menu_user0->ui->textBrowser->append(qyear);
                menu_user0->ui->textBrowser->append("urlS:");
               menu_user0->ui->textBrowser->append(qurlS);
                menu_user0->ui->textBrowser->append("urlM:");
               menu_user0-> ui->textBrowser->append(qurlM);
               menu_user0-> ui->textBrowser->append("urlN:");
                menu_user0->ui->textBrowser->append(qurlL);
               menu_user0-> ui->textBrowser->append("aveRank:");
               menu_user0-> ui->textBrowser->append(qaveRank);
               menu_user0-> ui->textBrowser->append("history comment:");
                for(int i=0;i<books.find(name)->second.comment.size();i++){
                    QString qcomment= QString::fromStdString(books.find(name)->second.comment[i]);
menu_user0-> ui->textBrowser->append(qcomment);
                    qcomment.clear();
                }
                menu_user0-> ui->textBrowser->append(" ");



                name.clear();
            }
            menu_user0->ui->textBrowser->append("we'll recommend you these friends:");
            for(int i=0;i<userrecommendlist.size();i++){
                name=userrecommendlist[i];
                QString qhimself,qtown,qcity,qcountry, qage;
                qhimself=QString::fromStdString(name);
                qtown=QString::fromStdString(users.find(name)->second.town);
                qcity=QString::fromStdString(users.find(name)->second.city);
                qcountry=QString::fromStdString(users.find(name)->second.country);
                char ren[64];
                sprintf(ren, "%d",users.find(name)->second.age );
                string agee(ren);
                qage=QString::fromStdString(agee.c_str());
                menu_user0->ui->textBrowser->append("name:");
                menu_user0->ui->textBrowser->append(qhimself);
                menu_user0->ui->textBrowser->append("town:");
                menu_user0->ui->textBrowser->append(qtown);
                menu_user0->ui->textBrowser->append("city:");
                menu_user0->ui->textBrowser->append(qcity);
                menu_user0->ui->textBrowser->append("country:");
                menu_user0->ui->textBrowser->append(qcountry);
                menu_user0->ui->textBrowser->append("age:");
                menu_user0->ui->textBrowser->append(qage);

                menu_user0-> ui->textBrowser->append(" ");
                name.clear();
            }

            bookrecommendlist.clear();
            userrecommendlist.clear();
        position=1;
    }
    else {
           QMessageBox::information(this,tr("tip"),
                            tr("ERROR"),QMessageBox::Ok);
        }

}
void signin::on_pushButton_clicked()
{
    signup* signup0=new signup();
    //close();
    signup0->show();
}



void signup::on_pushButton_clicked()
{
    QString qid,qpas0,qpas1,qcity,qpro,qnation;
    string id,pas0,pas1,city,pro,nation;
    int age;
    qid=ui->lineEdit->text();
    qpas0=ui->lineEdit_2->text();
    qpas1=ui->lineEdit_3->text();
    qcity=ui->lineEdit_4->text();
    qpro=ui->lineEdit_5->text();
    qnation=ui->lineEdit_6->text();
    id=qid.toStdString();
    pas0=qpas0.toStdString();
    pas1=qpas1.toStdString();
    city=qcity.toStdString();
    pro=qpro.toStdString();
    nation=qnation.toStdString();
    age=ui->spinBox->value();
    if(users.find(id)!=users.end()){
        QMessageBox::information(this,tr("tip"),
                          tr("can find it!you cannot signup!"),QMessageBox::Ok);return;
    }
    if(pas0==""||pas1==""){
        QMessageBox::information(this,tr("tip"),
                        tr("input your password please!"),QMessageBox::Ok);
       return;
    }
    if(pas0!=pas1){
         QMessageBox::information(this,tr("tip"),
                         tr("the password you input is different!"),QMessageBox::Ok);
        return;
    }
    User newuser;
    newuser.himself = id;
    newuser.town = city;
    newuser.city = pro;
    newuser.country = nation;
    newuser.age = age;
    newuser.password = pas0;
    users.insert(map<string, User>::value_type(newuser.himself, newuser));
    QMessageBox::information(this,tr("tip"),
                     tr("sign up complete!"),QMessageBox::Ok);
    close();
    signin* s=new signin();
    s->show();
}
void menu_user::on_pushButton_2_clicked()
{
    close();
    signin* s=new signin();
    s->show();
}
void menu_user::on_pushButton_3_clicked()
{
    //close();
    change_yourself* change0=new change_yourself();
    string pas=users[userid].password;
    string town=users[userid].town;
    string city=users[userid].city;
    string country=users[userid].country;
    int age=users[userid].age;
    QString qname=QString::fromStdString(userid);
    QString qpas=QString::fromStdString(pas);
    QString qtown=QString::fromStdString(town);
    QString qcity=QString::fromStdString(city);
    QString qcountry=QString::fromStdString(country);
    change0->ui->lineEdit_2->setText(qpas);
    change0->ui->lineEdit_3->setText(qpas);
    change0->ui->lineEdit_4->setText(qtown);
    change0->ui->lineEdit_5->setText(qcity);
    change0->ui->lineEdit_6->setText(qcountry);
    change0->ui->spinBox->setValue(age);
    change0->ui->label_8->setText(qname);

    change0->show();
}
void menu_user::on_pushButton_clicked()
{//差提示信息
    if(ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked()&&!ui->radioButton_3->isChecked()){
        ui->label_2->clear();

        QString qname=ui->lineEdit->text();
        string name=qname.toStdString();
        if(books.find(name)!=books.end()){
            ui->label_2->setText(qname);
            //ui->textBrowser->clear();
            QString qnam = QString::fromStdString(books.find(name)->second.title);
            ui->textBrowser->append(qnam);
            QString qauthor,qpublisher,qyear,qurlS, qurlM, qurlL,qaveRank,qsum;
            qauthor=QString::fromStdString(books.find(name)->second.author);
            qpublisher=QString::fromStdString(books.find(name)->second.publisher);
            qyear=QString::fromStdString(books.find(name)->second.year);
            qurlS=QString::fromStdString(books.find(name)->second.urlS);
            qurlM=QString::fromStdString(books.find(name)->second.urlM);
            qurlL=QString::fromStdString(books.find(name)->second.urlL);
            char ran[64];
            sprintf(ran, "%lf",books.find(name)->second.aveRank );
            string aveRANK(ran);
            qaveRank=QString::fromStdString(aveRANK.c_str());
            char rbn[64];
            sprintf(rbn, "%lf",books.find(name)->second.sum );
            string SUM(rbn);
            qsum=QString::fromStdString(SUM.c_str());
            ui->textBrowser->append("TITLE:");
            ui->textBrowser->append(qnam);
            ui->textBrowser->append("author:");
            ui->textBrowser->append(qauthor);
            ui->textBrowser->append("publisher:");
            ui->textBrowser->append(qpublisher);
            ui->textBrowser->append("year:");
            ui->textBrowser->append(qyear);
            ui->textBrowser->append("urlS:");
            ui->textBrowser->append(qurlS);
            ui->textBrowser->append("urlM:");
            ui->textBrowser->append(qurlM);
            ui->textBrowser->append("urlN:");
            ui->textBrowser->append(qurlL);
            ui->textBrowser->append("aveRank:");
            ui->textBrowser->append(qaveRank);
            ui->textBrowser->append("history comment:");
            for(int i=0;i<books.find(name)->second.comment.size();i++){
                QString qcomment= QString::fromStdString(books.find(name)->second.comment[i]);
                ui->textBrowser->append(qcomment);
                qcomment.clear();
            }
            ui->textBrowser->append(" ");
        }
        else{
            QMessageBox::information(this,tr("tip"),
                             tr("no find!"),QMessageBox::Ok);
        }
    }
    //ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
    else if(!ui->radioButton->isChecked()&&ui->radioButton_2->isChecked()&&!ui->radioButton_3->isChecked()){
        bool isfind=false;
        ui->label_2->clear();
        QString qname=ui->lineEdit->text();
        string name=qname.toStdString();
        map<string, Book>::iterator book_iter = books.begin();
        for(;book_iter!=books.end();book_iter++){
            if(book_iter->second.title==name){
                isfind=true;
                //ui->label_2->setText(qname);
                //ui->textBrowser->clear();
                QString qnam = QString::fromStdString(book_iter->second.title);
                ui->textBrowser->append(qnam);
                QString qauthor,qpublisher,qyear,qurlS, qurlM, qurlL,qaveRank,qsum;
                qauthor=QString::fromStdString(book_iter->second.author);
                qpublisher=QString::fromStdString(book_iter->second.publisher);
                qyear=QString::fromStdString(book_iter->second.year);
                qurlS=QString::fromStdString(book_iter->second.urlS);
                qurlM=QString::fromStdString(book_iter->second.urlM);
                qurlL=QString::fromStdString(book_iter->second.urlL);
                char ran[64];
                sprintf(ran, "%lf",book_iter->second.aveRank );
                string aveRANK(ran);
                qaveRank=QString::fromStdString(aveRANK.c_str());
                char rbn[64];
                sprintf(rbn, "%lf",book_iter->second.sum );
                string SUM(rbn);
                qsum=QString::fromStdString(SUM.c_str());
                ui->textBrowser->append("TITLE:");
                ui->textBrowser->append(qnam);
                ui->textBrowser->append("author:");
                ui->textBrowser->append(qauthor);
                ui->textBrowser->append("publisher:");
                ui->textBrowser->append(qpublisher);
                ui->textBrowser->append("year:");
                ui->textBrowser->append(qyear);
                ui->textBrowser->append("urlS:");
                ui->textBrowser->append(qurlS);
                ui->textBrowser->append("urlM:");
                ui->textBrowser->append(qurlM);
                ui->textBrowser->append("urlN:");
                ui->textBrowser->append(qurlL);
                ui->textBrowser->append("aveRank:");
                ui->textBrowser->append(qaveRank);
                ui->textBrowser->append("history comment:");

                ui->textBrowser->append(" ");
            }
        }
        if(!isfind){
            QMessageBox::information(this,tr("tip"),
                             tr("no find!"),QMessageBox::Ok);
        }
    }
    else if(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked()&&ui->radioButton_3->isChecked()){
        bool isfind=false;
        ui->label_2->clear();
        QString qname=ui->lineEdit->text();
        string name=qname.toStdString();
        map<string, Book>::iterator book_iter = books.begin();
        for(;book_iter!=books.end();book_iter++){
            if(book_iter->second.author==name){
                isfind=true;
                //ui->label_2->setText(qname);
                //ui->textBrowser->clear();
                QString qnam = QString::fromStdString(book_iter->second.title);
                ui->textBrowser->append(qnam);
                QString qauthor,qpublisher,qyear,qurlS, qurlM, qurlL,qaveRank,qsum;
                qauthor=QString::fromStdString(book_iter->second.author);
                qpublisher=QString::fromStdString(book_iter->second.publisher);
                qyear=QString::fromStdString(book_iter->second.year);
                qurlS=QString::fromStdString(book_iter->second.urlS);
                qurlM=QString::fromStdString(book_iter->second.urlM);
                qurlL=QString::fromStdString(book_iter->second.urlL);
                char ran[64];
                sprintf(ran, "%lf",book_iter->second.aveRank );
                string aveRANK(ran);
                qaveRank=QString::fromStdString(aveRANK.c_str());
                char rbn[64];
                sprintf(rbn, "%lf",book_iter->second.sum );
                string SUM(rbn);
                qsum=QString::fromStdString(SUM.c_str());
                ui->textBrowser->append("TITLE:");
                ui->textBrowser->append(qnam);
                ui->textBrowser->append("author:");
                ui->textBrowser->append(qauthor);
                ui->textBrowser->append("publisher:");
                ui->textBrowser->append(qpublisher);
                ui->textBrowser->append("year:");
                ui->textBrowser->append(qyear);
                ui->textBrowser->append("urlS:");
                ui->textBrowser->append(qurlS);
                ui->textBrowser->append("urlM:");
                ui->textBrowser->append(qurlM);
                ui->textBrowser->append("urlN:");
                ui->textBrowser->append(qurlL);
                ui->textBrowser->append("aveRank:");
                ui->textBrowser->append(qaveRank);
                ui->textBrowser->append("history comment:");

                ui->textBrowser->append(" ");
            }
        }
        if(!isfind){
            QMessageBox::information(this,tr("tip"),
                             tr("no find!"),QMessageBox::Ok);
        }
    }
    else{
        QMessageBox::information(this,tr("tip"),
                         tr("please choose a find mode!"),QMessageBox::Ok);
    }

       /* if(books.find(name)!=books.end()){
            ui->label_2->setText(qname);
            //ui->textBrowser->clear();
            QString qnam = QString::fromStdString(books.find(name)->second.title);
            ui->textBrowser->append(qnam);
            QString qauthor,qpublisher,qyear,qurlS, qurlM, qurlL,qaveRank,qsum;
            qauthor=QString::fromStdString(books.find(name)->second.author);
            qpublisher=QString::fromStdString(books.find(name)->second.publisher);
            qyear=QString::fromStdString(books.find(name)->second.year);
            qurlS=QString::fromStdString(books.find(name)->second.urlS);
            qurlM=QString::fromStdString(books.find(name)->second.urlM);
            qurlL=QString::fromStdString(books.find(name)->second.urlL);
            char ran[64];
            sprintf(ran, "%lf",books.find(name)->second.aveRank );
            string aveRANK(ran);
            qaveRank=QString::fromStdString(aveRANK.c_str());
            char rbn[64];
            sprintf(rbn, "%lf",books.find(name)->second.sum );
            string SUM(rbn);
            qsum=QString::fromStdString(SUM.c_str());
            ui->textBrowser->append("TITLE:");
            ui->textBrowser->append(qnam);
            ui->textBrowser->append("author:");
            ui->textBrowser->append(qauthor);
            ui->textBrowser->append("publisher:");
            ui->textBrowser->append(qpublisher);
            ui->textBrowser->append("year:");
            ui->textBrowser->append(qyear);
            ui->textBrowser->append("urlS:");
            ui->textBrowser->append(qurlS);
            ui->textBrowser->append("urlM:");
            ui->textBrowser->append(qurlM);
            ui->textBrowser->append("urlN:");
            ui->textBrowser->append(qurlL);
            ui->textBrowser->append("aveRank:");
            ui->textBrowser->append(qaveRank);
            ui->textBrowser->append("history comment:");
            for(int i=0;i<books.find(name)->second.comment.size();i++){
                QString qcomment= QString::fromStdString(books.find(name)->second.comment[i]);
                ui->textBrowser->append(qcomment);
                qcomment.clear();
            }
        }
        else{
            QMessageBox::information(this,tr("tip"),
                             tr("no find!"),QMessageBox::Ok);
        }
    }*/



}
void menu_user::on_pushButton_4_clicked()
{
    QString qname=ui->lineEdit->text();
    string name=qname.toStdString();
    if(users.find(name)!=users.end()){
        //users.find(str)->second.show();
        QString qhimself,qtown,qcity,qcountry, qage;
        qhimself=QString::fromStdString(name);
        qtown=QString::fromStdString(users.find(name)->second.town);
        qcity=QString::fromStdString(users.find(name)->second.city);
        qcountry=QString::fromStdString(users.find(name)->second.country);
        char ran[64];
        sprintf(ran, "%d",users.find(name)->second.age );
        string agee(ran);
        qage=QString::fromStdString(agee.c_str());
      // ui->textBrowser->append(qhimself);
        ui->textBrowser->append("name:");
        ui->textBrowser->append(qhimself);
        ui->textBrowser->append("town:");
        ui->textBrowser->append(qtown);
        ui->textBrowser->append("city:");
        ui->textBrowser->append(qcity);
        ui->textBrowser->append("country:");
        ui->textBrowser->append(qcountry);
        ui->textBrowser->append("age:");
        ui->textBrowser->append(qage);
        ui->textBrowser->append(" ");

    }
    else{
         QMessageBox::information(this,tr("tip"),
                         tr("no find!"),QMessageBox::Ok);
    }
}
void change_yourself::on_pushButton_clicked()
{
    QString qid,qpas0,qpas1,qcity,qpro,qnation;
    string id,pas0,pas1,city,pro,nation;
    int age;
    qpas0=ui->lineEdit_2->text();
    qpas1=ui->lineEdit_3->text();
    qcity=ui->lineEdit_4->text();
    qpro=ui->lineEdit_5->text();
    qnation=ui->lineEdit_6->text();
    //id=qid.toStdString();
    pas0=qpas0.toStdString();
    pas1=qpas1.toStdString();
    city=qcity.toStdString();
    pro=qpro.toStdString();
    nation=qnation.toStdString();
    age=ui->spinBox->value();
    if(pas0==""||pas1==""){
        QMessageBox::information(this,tr("tip"),
                        tr("input your password please!"),QMessageBox::Ok);
       return;
    }
    if(pas0!=pas1){
        QMessageBox::information(this,tr("tip"),
                         tr("the password you input is different!"),QMessageBox::Ok);
        return;
    }
    users.find(userid)->second.himself = id;
    users.find(userid)->second.town = city;
    users.find(userid)->second.city = pro;
    users.find(userid)->second.country = nation;
    users.find(userid)->second.age = age;
    users.find(userid)->second.password = pas0;
     QMessageBox::information(this,tr("tip"),
                     tr("change complete!"),QMessageBox::Ok);
    close();
}
void signup::on_pushButton_2_clicked()
{
    close();
    signin* signin0=new signin();
    signin0->show();
}
void change_yourself::on_pushButton_3_clicked()
{
    close();
}
void menu_admin::on_pushButton_clicked()
{
    if(ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked()&&!ui->radioButton_3->isChecked()){
        //ui->label_2->clear();
        QString qname=ui->lineEdit->text();
        string name=qname.toStdString();
        if(books.find(name)!=books.end()){
            //ui->label_2->setText(qname);
            //ui->textBrowser->clear();
            QString qnam = QString::fromStdString(books.find(name)->second.title);
            ui->textBrowser->append(qnam);
            QString qauthor,qpublisher,qyear,qurlS, qurlM, qurlL,qaveRank,qsum;
            qauthor=QString::fromStdString(books.find(name)->second.author);
            qpublisher=QString::fromStdString(books.find(name)->second.publisher);
            qyear=QString::fromStdString(books.find(name)->second.year);
            qurlS=QString::fromStdString(books.find(name)->second.urlS);
            qurlM=QString::fromStdString(books.find(name)->second.urlM);
            qurlL=QString::fromStdString(books.find(name)->second.urlL);
            char ran[64];
            sprintf(ran, "%lf",books.find(name)->second.aveRank );
            string aveRANK(ran);
            qaveRank=QString::fromStdString(aveRANK.c_str());
            char rbn[64];
            sprintf(rbn, "%lf",books.find(name)->second.sum );
            string SUM(rbn);
            qsum=QString::fromStdString(SUM.c_str());
            ui->textBrowser->append("TITLE:");
            ui->textBrowser->append(qnam);
            ui->textBrowser->append("author:");
            ui->textBrowser->append(qauthor);
            ui->textBrowser->append("publisher:");
            ui->textBrowser->append(qpublisher);
            ui->textBrowser->append("year:");
            ui->textBrowser->append(qyear);
            ui->textBrowser->append("urlS:");
            ui->textBrowser->append(qurlS);
            ui->textBrowser->append("urlM:");
            ui->textBrowser->append(qurlM);
            ui->textBrowser->append("urlN:");
            ui->textBrowser->append(qurlL);
            ui->textBrowser->append("aveRank:");
            ui->textBrowser->append(qaveRank);
            ui->textBrowser->append("history comment:");
            for(int i=0;i<books.find(name)->second.comment.size();i++){
                QString qcomment= QString::fromStdString(books.find(name)->second.comment[i]);
                ui->textBrowser->append(qcomment);
                qcomment.clear();
            }
             ui->textBrowser->append(" ");
        }
        else{
            QMessageBox::information(this,tr("tip"),
                             tr("no find!"),QMessageBox::Ok);
        }
    }
    //ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
    else if(!ui->radioButton->isChecked()&&ui->radioButton_2->isChecked()&&!ui->radioButton_3->isChecked()){
        bool isfind=false;
        //ui->label_2->clear();
        QString qname=ui->lineEdit->text();
        string name=qname.toStdString();
        map<string, Book>::iterator book_iter = books.begin();
        for(;book_iter!=books.end();book_iter++){
            if(book_iter->second.title==name){
                isfind=true;
                //ui->label_2->setText(qname);
                //ui->textBrowser->clear();
                QString qnam = QString::fromStdString(book_iter->second.title);
                ui->textBrowser->append(qnam);
                QString qauthor,qpublisher,qyear,qurlS, qurlM, qurlL,qaveRank,qsum;
                qauthor=QString::fromStdString(book_iter->second.author);
                qpublisher=QString::fromStdString(book_iter->second.publisher);
                qyear=QString::fromStdString(book_iter->second.year);
                qurlS=QString::fromStdString(book_iter->second.urlS);
                qurlM=QString::fromStdString(book_iter->second.urlM);
                qurlL=QString::fromStdString(book_iter->second.urlL);
                char ran[64];
                sprintf(ran, "%lf",book_iter->second.aveRank );
                string aveRANK(ran);
                qaveRank=QString::fromStdString(aveRANK.c_str());
                char rbn[64];
                sprintf(rbn, "%lf",book_iter->second.sum );
                string SUM(rbn);
                qsum=QString::fromStdString(SUM.c_str());
                ui->textBrowser->append("TITLE:");
                ui->textBrowser->append(qnam);
                ui->textBrowser->append("author:");
                ui->textBrowser->append(qauthor);
                ui->textBrowser->append("publisher:");
                ui->textBrowser->append(qpublisher);
                ui->textBrowser->append("year:");
                ui->textBrowser->append(qyear);
                ui->textBrowser->append("urlS:");
                ui->textBrowser->append(qurlS);
                ui->textBrowser->append("urlM:");
                ui->textBrowser->append(qurlM);
                ui->textBrowser->append("urlN:");
                ui->textBrowser->append(qurlL);
                ui->textBrowser->append("aveRank:");
                ui->textBrowser->append(qaveRank);
                ui->textBrowser->append("history comment:");
                for(int i=0;i<books.find(name)->second.comment.size();i++){
                    QString qcomment= QString::fromStdString(book_iter->second.comment[i]);
                    ui->textBrowser->append(qcomment);
                    qcomment.clear();
                }
                ui->textBrowser->append(" ");
            }
        }
        if(!isfind){
            QMessageBox::information(this,tr("tip"),
                             tr("no find!"),QMessageBox::Ok);
        }
    }
    else if(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked()&&ui->radioButton_3->isChecked()){
        bool isfind=false;
        //ui->label_2->clear();
        QString qname=ui->lineEdit->text();
        string name=qname.toStdString();
        map<string, Book>::iterator book_iter = books.begin();
        for(;book_iter!=books.end();book_iter++){
            if(book_iter->second.author==name){
                isfind=true;
                //ui->label_2->setText(qname);
                //ui->textBrowser->clear();
                QString qnam = QString::fromStdString(book_iter->second.title);
                ui->textBrowser->append(qnam);
                QString qauthor,qpublisher,qyear,qurlS, qurlM, qurlL,qaveRank,qsum;
                qauthor=QString::fromStdString(book_iter->second.author);
                qpublisher=QString::fromStdString(book_iter->second.publisher);
                qyear=QString::fromStdString(book_iter->second.year);
                qurlS=QString::fromStdString(book_iter->second.urlS);
                qurlM=QString::fromStdString(book_iter->second.urlM);
                qurlL=QString::fromStdString(book_iter->second.urlL);
                char ran[64];
                sprintf(ran, "%lf",book_iter->second.aveRank );
                string aveRANK(ran);
                qaveRank=QString::fromStdString(aveRANK.c_str());
                char rbn[64];
                sprintf(rbn, "%lf",book_iter->second.sum );
                string SUM(rbn);
                qsum=QString::fromStdString(SUM.c_str());
                ui->textBrowser->append("TITLE:");
                ui->textBrowser->append(qnam);
                ui->textBrowser->append("author:");
                ui->textBrowser->append(qauthor);
                ui->textBrowser->append("publisher:");
                ui->textBrowser->append(qpublisher);
                ui->textBrowser->append("year:");
                ui->textBrowser->append(qyear);
                ui->textBrowser->append("urlS:");
                ui->textBrowser->append(qurlS);
                ui->textBrowser->append("urlM:");
                ui->textBrowser->append(qurlM);
                ui->textBrowser->append("urlN:");
                ui->textBrowser->append(qurlL);
                ui->textBrowser->append("aveRank:");
                ui->textBrowser->append(qaveRank);
                ui->textBrowser->append("history comment:");
                for(int i=0;i<books.find(name)->second.comment.size();i++){
                    QString qcomment= QString::fromStdString(book_iter->second.comment[i]);
                    ui->textBrowser->append(qcomment);
                    qcomment.clear();
                }
                ui->textBrowser->append(" ");
            }
        }
        if(!isfind){
            QMessageBox::information(this,tr("tip"),
                             tr("no find!"),QMessageBox::Ok);
        }
    }
    else{
        QMessageBox::information(this,tr("tip"),
                         tr("please choose a find mode!"),QMessageBox::Ok);
    }

}
void menu_admin::on_pushButton_4_clicked()
{
    QString qname=ui->lineEdit->text();
    string name=qname.toStdString();
    if(users.find(name)!=users.end()){
        QString qhimself,qtown,qcity,qcountry, qage;
        qhimself=QString::fromStdString(users.find(name)->second.himself);
        qtown=QString::fromStdString(users.find(name)->second.town);
        qcity=QString::fromStdString(users.find(name)->second.city);
        qcountry=QString::fromStdString(users.find(name)->second.country);
        char ran[64];
        sprintf(ran, "%d",users.find(name)->second.age );
        string agee(ran);
        qage=QString::fromStdString(agee.c_str());
        ui->textBrowser->append("name:");
        ui->textBrowser->append(qhimself);
        ui->textBrowser->append("town:");
        ui->textBrowser->append(qtown);
        ui->textBrowser->append("city:");
        ui->textBrowser->append(qcity);
        ui->textBrowser->append("country:");
        ui->textBrowser->append(qcountry);
        ui->textBrowser->append("age:");
        ui->textBrowser->append(qage);
        ui->textBrowser->append(" ");
    }
    else{
        QMessageBox::information(this,tr("tip"),
                         tr("no find!"),QMessageBox::Ok);
    }
}
void menu_admin::on_pushButton_9_clicked()
{
    close();
    signin* signin0=new signin();
    signin0->show();
}
void menu_admin::on_pushButton_2_clicked()
{
    book_add* bookadd0=new book_add();
    bookadd0->show();
}
void menu_admin::on_pushButton_5_clicked()
{
        book_change* bookchange0=new book_change();
        bookchange0->show();
}
void book_change::on_pushButton_clicked()
{
    string bookid;
    QString qid,qtitle,qauthor,qpublisher,qurls,qurlm,qurll;
    string id,title,author,publisher,urls,urlm,urll;
    int nyear;
    qid=ui->lineEdit_8->text();
    //qid=ui->lineEdit_8->text();
    qtitle=ui->lineEdit_2->text();
    qauthor=ui->lineEdit_3->text();
    qpublisher=ui->lineEdit_7->text();
    qurls=ui->lineEdit_4->text();
    qurlm=ui->lineEdit_5->text();
    qurll=ui->lineEdit_6->text();
    bookid=qid.toStdString();
    //id=qid.toStdString();
    title=qtitle.toStdString();
    author=qauthor.toStdString();
    publisher=qpublisher.toStdString();
    urls=qurls.toStdString();
    urlm=qurlm.toStdString();
    urll=qurll.toStdString();
    nyear=ui->spinBox->value();
    char ran[64];
    sprintf(ran, "%d",nyear );
    string year(ran);
    if(books.find(bookid)!=books.end()){
        books.find(bookid)->second.rechangebook("title",title);
        books.find(bookid)->second.rechangebook("author",author);
        books.find(bookid)->second.rechangebook("publisher",publisher);
        books.find(bookid)->second.rechangebook("urlS",urls);
        books.find(bookid)->second.rechangebook("urlM",urlm);
        books.find(bookid)->second.rechangebook("urlL",urll);
        books.find(bookid)->second.year=year;
    }
   QMessageBox::information(this,tr("tip"),
                     tr("change complete!"),QMessageBox::Ok);
    close();
}
void book_change::on_pushButton_2_clicked()
{
    string bookid;
    QString qbookid;
    qbookid=ui->lineEdit_8->text();
    bookid=qbookid.toStdString();
    if(books.find(bookid)!=books.end()){
       QMessageBox::information(this,tr("tip"),
                         tr("find it!you can change!"),QMessageBox::Ok);
       string title=books[bookid].title;
       string author=books[bookid].author;
       string publisher=books[bookid].publisher;
       string urls=books[bookid].urlS;
       string urlm=books[bookid].urlM;
       string urll=books[bookid].urlL;
       int year=atoi(books[bookid].year.c_str());
       QString qtitle=QString::fromStdString(title);
       QString qauthor=QString::fromStdString(author);
       QString qpublisher=QString::fromStdString(publisher);
       QString qurls=QString::fromStdString(urls);
       QString qurlm=QString::fromStdString(urlm);
       QString qurll=QString::fromStdString(urll);
       ui->lineEdit_2->setText(qtitle);
       ui->lineEdit_3->setText(qauthor);
       ui->lineEdit_7->setText(qpublisher);
       ui->lineEdit_4->setText(qurls);
       ui->lineEdit_5->setText(qurlm);
       ui->lineEdit_6->setText(qurll);
       ui->spinBox->setValue(year);
    }
    else{
        QMessageBox::information(this,tr("tip"),
                         tr("cannot find it!you cannot change!"),QMessageBox::Ok);
    }
}
void book_add::on_pushButton_clicked()
{
    QString qid,qtitle,qauthor,qpublisher,qurls,qurlm,qurll;
    string id,title,author,publisher,urls,urlm,urll;
    int nyear;
    //qid=ui->lineEdit->text();
    qid=ui->lineEdit_8->text();
    qtitle=ui->lineEdit_2->text();
    qauthor=ui->lineEdit_3->text();
    qpublisher=ui->lineEdit_7->text();
    qurls=ui->lineEdit_4->text();
    qurlm=ui->lineEdit_5->text();
    qurll=ui->lineEdit_6->text();
    //id=qid.toStdString();
    id=qid.toStdString();
    title=qtitle.toStdString();
    author=qauthor.toStdString();
    publisher=qpublisher.toStdString();
    urls=qurls.toStdString();
    urlm=qurlm.toStdString();
    urll=qurll.toStdString();
    nyear=ui->spinBox->value();
    char ran[64];
    sprintf(ran, "%d",nyear );
    string year(ran);
    if(books.find(id)!=books.end()){
        QMessageBox::information(this,tr("tip"),
                         tr("find an existing one!you cannot change!"),QMessageBox::Ok);
        return ;

    }
    Book newbook;
    newbook.id = id;
    newbook.title = title;
    newbook.author = author;
    newbook.publisher = publisher;
    newbook.year = year;
    newbook.urlS = urls;
    newbook.urlM = urlm;
    newbook.urlL = urll;
    books.insert(map<string, Book>::value_type(newbook.id, newbook));
    QMessageBox::information(this,tr("tip"),
                     tr("add complete!"),QMessageBox::Ok);
    close();
}
void menu_admin::on_pushButton_3_clicked()
{
    bool ok;
    // 获取字符串
    QString qid = QInputDialog::getText(this,tr("input"),
              tr("input the id:"),QLineEdit::Normal,tr("admin"),&ok);
    string id=qid.toStdString();
    if(books.find(id)!=books.end()){
        books[id].deletebook();
    }
    else{
        QMessageBox::information(this,tr("tip"),
                         tr("cannot find so you cannot delete!"),QMessageBox::Ok);
    }
}
void menu_admin::on_pushButton_7_clicked()
{
    adduser* adduser0=new adduser();
    adduser0->show();
}
void adduser::on_pushButton_clicked()
{
    QString qid,qpas0,qpas1,qcity,qpro,qnation;
    string id,pas0,pas1,city,pro,nation;
    int age;
    qid=ui->lineEdit->text();
    qpas0=ui->lineEdit_2->text();
    qpas1=ui->lineEdit_3->text();
    qcity=ui->lineEdit_4->text();
    qpro=ui->lineEdit_5->text();
    qnation=ui->lineEdit_6->text();
    id=qid.toStdString();
    pas0=qpas0.toStdString();
    pas1=qpas1.toStdString();
    city=qcity.toStdString();
    pro=qpro.toStdString();
    nation=qnation.toStdString();
    age=ui->spinBox->value();
    if(pas0!=pas1){
      QMessageBox::information(this,tr("tip"),
                         tr("the password you input is different!"),QMessageBox::Ok);
        return;
    }
    if(users.find(id)!=users.end()){
        QMessageBox::information(this,tr("tip"),
                           tr("there is an exsiting one.you cannot add!"),QMessageBox::Ok);
        return;
    }
    User newuser;
    newuser.himself = id;
    newuser.town = city;
    newuser.city = pro;
    newuser.country = nation;
    newuser.age = age;
    newuser.password = pas0;
    users.insert(map<string, User>::value_type(newuser.himself, newuser));
   QMessageBox::information(this,tr("tip"),
                     tr("sign up complete!"),QMessageBox::Ok);
    close();
}
void menu_admin::on_pushButton_6_clicked()
{
    bool ok;
    // 获取字符串
    QString qid = QInputDialog::getText(this,tr("input"),
              tr("input the id:"),QLineEdit::Normal,tr("admin"),&ok);
    string id=qid.toStdString();
    if(users.find(id)!=users.end()){
        users[id].deleteuser();
    }
    else{
        QMessageBox::information(this,tr("tip"),
                         tr("cannot find so you cannot delete!"),QMessageBox::Ok);
    }
}
void user_change::on_pushButton_clicked()
{
    QString qid,qpas0,qpas1,qcity,qpro,qnation;
    string id,pas0,pas1,city,pro,nation;
    int age;
    qid=ui->lineEdit->text();
    qpas0=ui->lineEdit_2->text();
    qpas1=ui->lineEdit_3->text();
    qcity=ui->lineEdit_4->text();
    qpro=ui->lineEdit_5->text();
    qnation=ui->lineEdit_6->text();
    id=qid.toStdString();
    pas0=qpas0.toStdString();
    pas1=qpas1.toStdString();
    city=qcity.toStdString();
    pro=qpro.toStdString();
    nation=qnation.toStdString();
    age=ui->spinBox->value();
    if(users.find(id)!=users.end()){
        users.find(id)->second.password=pas0;
        users.find(id)->second.town=city;
        users.find(id)->second.city=pro;
        users.find(id)->second.country=nation;
        users.find(id)->second.age=age;
    }

    if(pas0==""||pas1==""){
        QMessageBox::information(this,tr("tip"),
                        tr("input your password please!"),QMessageBox::Ok);
       return;
    }

    if(pas0!=pas1){
         QMessageBox::information(this,tr("tip"),
                         tr("the password you input is different!"),QMessageBox::Ok);
        return;
    }
   QMessageBox::information(this,tr("tip"),
                     tr("change complete!"),QMessageBox::Ok);
    close();
}
void menu_admin::on_pushButton_8_clicked()
{
    user_change* userchange0=new user_change();
    userchange0->show();

}
void user_change::on_pushButton_2_clicked()
{
    string userid;
    QString quserid;
    quserid=ui->lineEdit->text();
    userid=quserid.toStdString();
    if(users.find(userid)!=users.end()){
      QMessageBox::information(this,tr("tip"),
                         tr("find it!you can change!"),QMessageBox::Ok);
      string pas=users[userid].password;
      string town=users[userid].town;
      string city=users[userid].city;
      string country=users[userid].country;
      int age=users[userid].age;
      QString qname=QString::fromStdString(userid);
      QString qpas=QString::fromStdString(pas);
      QString qtown=QString::fromStdString(town);
      QString qcity=QString::fromStdString(city);
      QString qcountry=QString::fromStdString(country);
      ui->lineEdit_2->setText(qpas);
      ui->lineEdit_3->setText(qpas);
      ui->lineEdit_4->setText(qtown);
      ui->lineEdit_5->setText(qcity);
      ui->lineEdit_6->setText(qcountry);
      ui->spinBox->setValue(age);
      //ui->label_8->setText(qname);

    }
    else{
       QMessageBox::information(this,tr("tip"),
                         tr("cannot find it!you cannot change!"),QMessageBox::Ok);
    }
}
void menu_user::on_pushButton_5_clicked()
{
    QString qid=ui->label_2->text();
    string id=qid.toStdString();
    int r;//评分

    r=ui->spinBox->value();
    if(id==""){
        QMessageBox::information(this,tr("tip"),
                          tr("please choose a book first!"),QMessageBox::Ok);
        return ;
    }
    if(books.find(id)!=books.end()){
            bool read=false;
        map<string, int>::iterator iterp=users.find(userid)->second.booksRead.begin();
                        for(;iterp!=users.find(userid)->second.booksRead.end();iterp++){
                            if(iterp->first==id){
                                QMessageBox::information(this,tr("tip"),
                                                  tr("you have ever scored it!"),QMessageBox::Ok);read=true;
                                return ;
                            }
                        }
           users.find(userid)->second.rank( r, id);
           QMessageBox::information(this,tr("tip"),
                            tr("score success!"),QMessageBox::Ok);
    }
    else{
        QMessageBox::information(this,tr("tip"),
                         tr("cannot find the book!please input it again!"),QMessageBox::Ok);
    }
}
void menu_user::on_pushButton_6_clicked()
{
    QString qcomment=ui->textEdit->toPlainText();
    string comment=qcomment.toStdString();
    QString qid=ui->label_2->text();
    string id=qid.toStdString();
    if(books.find(id)!=books.end()){
           users.find(userid)->second.add_comment(books[id], comment);
        QMessageBox::information(this,tr("tip"),
                            tr("comment success!"),QMessageBox::Ok);
    }
    else{
        QMessageBox::information(this,tr("tip"),
                         tr("cannot find the book!please input it again!"),QMessageBox::Ok);
    }
}
void menu_admin::closeEvent(QCloseEvent *event){
    writeback();
}
void menu_user::closeEvent(QCloseEvent *event){
    writeback();
}
/*void signin::closeEvent(QCloseEvent *event){
    writeback();
}*/
void signup::closeEvent(QCloseEvent *event){
    writeback();
}



void adduser::on_pushButton_2_clicked()
{
    string userid;
    QString quserid;
    quserid=ui->lineEdit->text();
    userid=quserid.toStdString();
    if(users.find(userid)!=users.end()){
        QMessageBox::information(this,tr("tip"),
                          tr("can find it!you cannot add!"),QMessageBox::Ok);

    }
    else{
       QMessageBox::information(this,tr("tip"),
                         tr("you can add!"),QMessageBox::Ok);
    }
}
void signin::on_radioButton_clicked()
{
    if(ui->radioButton->isChecked())
    ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);

}


void book_add::on_pushButton_2_clicked()
{
    string bookid;
    QString qbookid;
    qbookid=ui->lineEdit_8->text();
    bookid=qbookid.toStdString();
    if(books.find(bookid)!=books.end()){
        QMessageBox::information(this,tr("tip"),
                          tr("can find an existing one!you cannot add!"),QMessageBox::Ok);

    }
    else{
       QMessageBox::information(this,tr("tip"),
                         tr("cannot find it!you can add!"),QMessageBox::Ok);
    }
}
void signup::on_pushButton_3_clicked()
{
    QString qname=ui->lineEdit->text();
    string name=qname.toStdString();
    if(users.find(name)!=users.end()){
        QMessageBox::information(this,tr("tip"),
                          tr("can find it!you cannot signup!"),QMessageBox::Ok);
    }
    else{
        QMessageBox::information(this,tr("tip"),
                          tr("cannot find it!you can up!"),QMessageBox::Ok);
    }
}
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    signin* s=new signin();
    s->show();
    readFile();
    Rank();
    Administrators.insert(map<string, string>::value_type("mzy", "q317958662"));
    Administrators.insert(map<string, string>::value_type("lsq", "????"));
    Administrators.insert(map<string, string>::value_type("wxb", "????"));
    Administrators.insert(map<string, string>::value_type("lxy", "????"));
    return a.exec();
}
