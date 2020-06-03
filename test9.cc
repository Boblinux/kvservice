#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include <stdio.h>
#include <string>
#include <map>
#include <mutex>

#define MAXMEM  1000000

std::map<std::string, std::string> g_kvmem;
std::mutex g_mutex;

void depatchstr(std::string inputstr, const muduo::TcpConnectionPtr& conn)
{
  int len = (int)inputstr.size();
  int index_1 = 0, index_2 = 0;
  int index_3 = 0, index_4 = 0;
  std::string operate = "";
  std::string name = "";
  std::string id = "";

  for(int i=0; i<len; i++) 
  {
    if(inputstr[i] == '(')
    {
      index_2 = i+1;
    }
    if(inputstr[i] == ':')
    {
      index_3 = i+1;
    }
    if(inputstr[i] == ')')
    {
      index_4 = i;
      
      operate = inputstr.substr(index_1, index_2-index_1-1);
      if(index_1 == index_3 && index_2 == index_4 && operate == "getall") 
      {
        std::map<std::string, std::string>::iterator iter;
        iter = g_kvmem.begin();
        while(iter != g_kvmem.end()) {
          conn->send(iter->first+" : "+iter->second+'\n');
          iter++;
        }
        conn->send("getall success!\n");

      }
      
      if(index_1 == index_3 && index_2 != index_4 && operate == "get") 
      {
        name = inputstr.substr(index_2, index_4-index_2);
        if(g_kvmem[name] != "")
        {
          conn->send(name+" : "+ g_kvmem[name]+'\n');
        }
        else 
        {
          conn->send(name+" : have not id.\n");
        }

      }

      if(index_1 != index_3 && index_2 != index_4 && operate == "put") 
      {
        name = inputstr.substr(index_2, index_3-index_2-1);
        id = inputstr.substr(index_3, index_4-index_3);
        g_kvmem[name] = id;
        printf("put: %s : %s\n", (char*)name.data(), (char*)id.data());
        //std::cout << "put: " << name << " : " << id << std::endl;
      }

      index_1 = index_2 = index_3 = index_4 = i+1;

      operate.clear();
      name.clear();
      id.clear();
    }
    
  }
}


void onConnection(const muduo::TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): tid=%d new connection [%s] from %s\n",
           muduo::CurrentThread::tid(),
           conn->name().c_str(),
           conn->peerAddress().toHostPort().c_str());
  }
  else
  {
    printf("onConnection(): tid=%d connection [%s] is down\n",
           muduo::CurrentThread::tid(),
           conn->name().c_str());
  }
}

void onMessage(const muduo::TcpConnectionPtr& conn,
               muduo::Buffer* buf,
               muduo::Timestamp receiveTime)
{
  
  printf("onMessage(): tid=%d received %zd bytes from connection\n",
         muduo::CurrentThread::tid(),
         buf->readableBytes());

  g_mutex.lock();
  //Empty when the map exceeds a certain size
  if(g_kvmem.size() >= MAXMEM)
  {
    g_kvmem.clear();
    std::cout << "clear memory......" << std::endl;
  }
  std::string inputstr = buf->retrieveAsString();

  depatchstr(inputstr, conn);

  /*
  while(inputstr != ")" && inputstr != "") 
  {
    std::string operate = getoperate(inputstr);
    //std::cout << "operate: " << operate << std::endl;
    if(operate == "getall") 
    {
      std::map<std::string, std::string>::iterator iter;
      iter = g_kvmem.begin();
      while(iter != g_kvmem.end()) {
        conn->send(iter->first+" : "+iter->second+'\n');
        iter++;
      }
      conn->send("getall success!\n");
    } else if(operate == "get") 
    {
      std::string name = getnamestr(inputstr);
      if(g_kvmem[name] != "")
      {
        conn->send(name+" : "+ g_kvmem[name]+'\n');
      }
      else 
      {
        conn->send(name+" : have not id.\n");
      }
    } else if(operate == "put")
    {
      std::string name = getnamestr(inputstr);
      std::string id = getidstr(inputstr);
      g_kvmem[name] = id;
      conn->send("put success!\n");
    } else
    {
      conn->send(operate);
    }
    std::cout << "input: " << inputstr << std::endl;
  }*/
  g_mutex.unlock();
}

int main(int argc, char* argv[])
{
  printf("main(): pid = %d\n", getpid());

  muduo::InetAddress listenAddr(9981);
  muduo::EventLoop loop;

  muduo::TcpServer server(&loop, listenAddr);
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  if (argc > 1) {
    server.setThreadNum(atoi(argv[1]));
  }
  server.start();

  loop.loop();
}
