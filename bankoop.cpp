#include <iostream>
#include <memory> // unique_ptr kullanılacak , polymorphism + memory leak olmaması için
//unique ptr scope bitince nesneyi otomatik siler daha güvenlidir
#include <vector>
#include <string>
using namespace std;


class Account {
  //alt sınıf erişebilsin diye protected -> OOP de encapsulation dengesi
  //encapsulation: veriyi koruma + kontrollü erişim
  protected:
    int id;
    double balance;

  public:
  //consturctor: kullanarak parametreyle gelen id =class içindeki id yapılır,nesne oluşturulurken çalışan fonksiyon
    Account(int id,double balance): id(id),balance(balance){}
//virtual destructor: hangi classtan türemiş olursa olsun doğru versiyon çalışsın
    virtual ~Account() = default;
  //okuma fonksiyonları değişmez bu yüzden const yazılır
    int getId() const {
      return id;
    }
    double getBalance() const {
      return balance;
    }
    //her hesap tipi kendini tanıtmalı checking veya saving hesabı tipi belli olmadığı için 0
    virtual string type() const = 0;
    // bazı hesap türleri limit koyabilir bazıları fee kesebilir
    virtual void deposit(double amount) = 0;
    //para çekme başarılı mı değil mi bilgisi
    virtual bool withdraw(double amount) = 0;
    //aynı func farklı class için farklı davranır polymorphism -> savings de faiz ekler, checkingde fee keser
    virtual void monthEnd() = 0;
    //svaing farklı checking farklı şey yazdırır o yüzden sıfıra eşit
    virtual void print() const = 0;

};// bu class ın amacı = Bir account olmak istiyorsan bu bilgileri yazmak zorundaısn


class SavingsAccount : public Account {
  private:
    double interestRate;

  public:
  SavingsAccount(int id, double balance, double rate)
    : Account(id, balance), interestRate(rate) {}

  string type() const override {
      return "SAVINGS";
    }

  void deposit(double amount) override {
    if (amount <= 0)
      throw invalid_argument("Amount must be positive.");
    balance += amount;
    }

  bool withdraw(double amount) override {
      if (amount <= 0)
        throw invalid_argument("Amount must be positive.");
      if (amount > balance)
        return false;
      balance -= amount;
      return true;
    }

  void monthEnd() override {
      if (balance > 0)
        balance += balance * interestRate;
    }

  void print() const override {
     cout << "[SAVINGS] ID=" << id
      << " Balance=" << balance
      << " Rate=" << interestRate << endl;
    }
};
class CheckingAccount : public Account {

  private:
  double overdraftLimit;

  public:
   CheckingAccount(int id,double balance, double overdraftLimit)
   : Account(id,balance), overdraftLimit(overdraftLimit){}

   string type() const override {
    return "CHECKING";
   }
   void deposit(double amount) override {
    if(amount <= 0)
      throw invalid_argument("Amount must be positive. ");
    balance += amount;
   }
   bool withdraw(double amount) override {
    if(amount <= 0)
      throw invalid_argument("Amount must be positive. ");
    if(balance-amount < -overdraftLimit)
      return false;
    balance -=amount;
    return true;
   }
   void monthEnd() override {
    if(balance < 0)
      balance-=5;
   }

  void print() const override {
    cout << "[CHECKING] ID= "<< id 
    << " Balance= " << balance
    << " Overdraft= " << overdraftLimit << endl;
  }
};

  Account* findAccount(vector<unique_ptr<Account>>& accounts, int id) {
    for (auto& acc : accounts) {
      if (acc->getId() == id) return acc.get();
  }
    return nullptr;
}

  bool idExists(const vector<unique_ptr<Account>>& accounts, int id) {
    for (const auto& acc : accounts) {
      if (acc->getId() == id) return true;
  }
    return false;
}


int main(){
vector<unique_ptr<Account>> accounts;
while(true){
  cout << "\n=== Welcome To Mini Bank ===\n";
  cout << "1)Create Savings Account\n";
  cout << "2)Create Checking Account\n";
  cout << "3)List Accounts\n";
  cout << "4) Deposit\n";
  cout << "5) Withdraw\n";
  cout << "6) Month End (apply interest/fees)\n";
  cout << "7) Transfer (from -> to)\n";
  cout << "0)Exit\n";
  cout << "Select:";

  int choice;
  cin >> choice;

  if(choice == 0){
    cout << "Bye!\n";
    break;
  }

  if(choice == 1){
    int id;
    double balance,rate;
    cout << "Enter ID, initial balance, interest rate";
    cin >> id >> balance >> rate;
    if (idExists(accounts, id)) {
      cout << "This ID already exists. Try a different ID.\n";
      continue;
    }

    accounts.push_back(make_unique<SavingsAccount>(id,balance,rate));
    cout << "Saving account created.\n";
  }
  else if(choice==2){
    int id;
    double balance, overdraft;
    cout << "Enter ID, initial balance, overdraft limit: ";
    cin >> id >> balance >> overdraft;
    if (idExists(accounts, id)) {
      cout << "This ID already exists. Try a different ID.\n";
      continue;
    }

    accounts.push_back(make_unique<CheckingAccount>(id, balance, overdraft));
    cout << "Checking account created. \n";
  }
  else if (choice == 3){
    if(accounts.empty()) {
      cout << "No accounts yet.\n";

    }else {
      cout << "\n--- Accounts ---\n";
      for(const auto& acc: accounts) {
        acc->print();
      }
    }

  } 
  else if (choice == 4) {
    int id;
    double amount;
    cout << "Enter Account ID and amount to deposit: ";
    cin >> id >> amount;

    Account* acc = findAccount(accounts, id);
    if (!acc) {
      cout << "Account not found.\n";
  } else {
      try {
      acc->deposit(amount);
      cout << "Deposit successful. New balance: " << acc->getBalance() << "\n";
    } catch (const exception& e) {
      cout << "Error: " << e.what() << "\n";
    }
  }
  } else if (choice == 5) {
      int id;
      double amount;
      cout << "Enter Account ID and amount to withdraw: ";
      cin >> id >> amount;

    Account* acc = findAccount(accounts, id);
      if (!acc) {
      cout << "Account not found.\n";
  }   else {
      try {
      bool ok = acc->withdraw(amount);
      if (ok) {
        cout << "Withdraw successful. New balance: " << acc->getBalance() << "\n";
    } else {
        cout << "Withdraw failed (insufficient funds/overdraft limit).\n";
      }
    } catch (const exception& e) {
      cout << "Error: " << e.what() << "\n";
    }
  }
    } else if (choice == 6) {
        if (accounts.empty()) {
        cout << "No accounts to process.\n";
  }     else {
          for (auto& acc : accounts) {
            acc->monthEnd();   // POLYMORPHISM: Savings faiz, Checking fee
          }
        cout << "Month-end applied to all accounts.\n";
  }
} else if (choice == 7) {
  int fromId, toId;
  double amount;

  cout << "Enter FROM account ID, TO account ID, amount: ";
  cin >> fromId >> toId >> amount;

  if (fromId == toId) {
    cout << "Transfer failed: FROM and TO accounts cannot be the same.\n";
    continue;
  }

  Account* fromAcc = findAccount(accounts, fromId);
  Account* toAcc   = findAccount(accounts, toId);

  if (!fromAcc || !toAcc) {
    cout << "Transfer failed: account not found.\n";
    continue;
  }

  try {
    // 1) Withdraw from source
    bool ok = fromAcc->withdraw(amount);
    if (!ok) {
      cout << "Transfer failed: insufficient funds / overdraft limit.\n";
      continue;
    }

    // 2) Deposit to destination
    toAcc->deposit(amount);

    cout << "Transfer successful.\n";
    cout << "FROM new balance: " << fromAcc->getBalance() << "\n";
    cout << "TO   new balance: " << toAcc->getBalance() << "\n";

  } catch (const exception& e) {
    cout << "Error: " << e.what() << "\n";
  }
} else {
    cout << "Invalid option.\n";
  }

}

return 0;

}