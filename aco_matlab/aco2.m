%% ����������(TSP)�Ż�
%% ��ջ�������
clear all
clc

%% ��������
citys = ceil(rand(30,2)*10000)   %��������ĳ�������


%% ������м��໥����
fprintf('Computing Distance Matrix... \n');
n = size(citys,1);
D = zeros(n,n);
for i = 1:n
    for j = 1:n
        if i ~= j
            D(i,j) = sqrt(sum((citys(i,:) - citys(j,:)).^2));
        else
            D(i,j) = 1e-4;      
        end
    end    
end

%% ��ʼ������
fprintf('Initializing Parameters... \n');
m = 50;                              % ��������
alpha = 1;                           % ��Ϣ����Ҫ�̶�����
beta = 5;                            % ����������Ҫ�̶�����
rho = 0.1;                           % ��Ϣ�ػӷ�����
Q = 1;                               % ��ϵ��
Eta = 1./D;                          % ��������
Tau = ones(n,n);                     % ��Ϣ�ؾ���
Table = zeros(m,n);                  % ·����¼��
tabu_list =  zeros(m,n);     
iter = 1;                            % ����������ֵ
iter_max = 150;                      % ���������� 
Route_best = zeros(iter_max,n);      % �������·��       
Length_best = zeros(iter_max,1);     % �������·���ĳ���  
Length_ave = zeros(iter_max,1);      % ����·����ƽ������  

%% ����Ѱ�����·��
figure;
while iter <= iter_max
    fprintf('������%d��\n',iter);
    % ��������������ϵ�������
      start = zeros(m,1);
      for i = 1:m
          temp = randperm(n);
          start(i) = temp(1);
      end
      Table(:,1) = start; 
      % ������ռ�
      citys_index = 1:n;
      % �������·��ѡ��
      for i = 1:m
          % �������·��ѡ��
         for j = 2:n
             tabu = Table(i,1:(j - 1));           % �ѷ��ʵĳ��м���(���ɱ�)
             allow_index = ~ismember(citys_index,tabu);
             allow = citys_index(allow_index);  % �����ʵĳ��м���
             P = allow;
             % ������м�ת�Ƹ���
             for k = 1:length(allow)
                 P(k) = Tau(tabu(end),allow(k))^alpha * Eta(tabu(end),allow(k))^beta;
             end
             P = P/sum(P);
             % ���̶ķ�ѡ����һ�����ʳ���
             Pc = cumsum(P);     
            target_index = find(Pc >= rand); 
            target = allow(target_index(1));
            Table(i,j) = target;
         end
      end
      % ����������ϵ�·������
      Length = zeros(m,1);
      for i = 1:m
          Route = Table(i,:);
          for j = 1:(n - 1)
              Length(i) = Length(i) + D(Route(j),Route(j + 1));
          end
          Length(i) = Length(i) + D(Route(n),Route(1));
      end
      % �������·�����뼰ƽ������
      if iter == 1
          [min_Length,min_index] = min(Length);
          Length_best(iter) = min_Length;  
          Length_ave(iter) = mean(Length);
          Route_best(iter,:) = Table(min_index,:);
      else
          [min_Length,min_index] = min(Length);
          Length_best(iter) = min(Length_best(iter - 1),min_Length);
          Length_ave(iter) = mean(Length);
          if Length_best(iter) == min_Length
              Route_best(iter,:) = Table(min_index,:);
          else
              Route_best(iter,:) = Route_best((iter-1),:);
          end
      end
      % ������Ϣ��
      Delta_Tau = zeros(n,n);
Delta_pheromone = zeros(n,n);

      % ������ϼ���
      for i = 1:m
          % ������м���
          for j = 1:(n - 1)
              Delta_Tau(Table(i,j),Table(i,j+1)) = Delta_Tau(Table(i,j),Table(i,j+1)) + Q/Length(i);
 Delta_pheromone(Table(i,j),Table(i,j+1))=Delta_pheromone(Table(i,j),Table(i,j+1))+Q/Length(i);%Length(i)Ϊ��iֻ�����ڱ���ѭ�������߹���·������

          end
          Delta_Tau(Table(i,n),Table(i,1)) = Delta_Tau(Table(i,n),Table(i,1)) + Q/Length(i);

Delta_pheromone(tabu_list(i,n),tabu_list(i,1))=Delta_pheromone(tabu_list(i,n),tabu_list(i,1))+Q/total_length(i);%�ѵ�iֻ�����ڱ���ѭ����������·�����ͷŵ���Ϣ�ؾ��ۼ���ȥ
   	     
      end
      Tau = (1-rho) * Tau + Delta_Tau;
    % ����������1�����·����¼��
m_elitist = 2%��Ӣ���ϵĸ���
Delta_pheromone_elitists=zeros(n,n);
for j=1:(n-1)
    	Delta_pheromone_elitists(Table(i,j),Table(i,j+1))=Delta_pheromone_elitists(Table(i,j),Table(i,j+1))+m_elitist*Q/Length_best(iter+1);
%Q/Length_best(iter+1)Ϊ����ѭ���о�Ӣ�����ҳ�������·���ĳ���
    end
    Delta_pheromone_elitists(Table(i,n),Table(i,1))=Delta_pheromone_elitists(Table(i,n),Table(i,1))+m_elitist*Q/Length_best(iter+1);%������·����ÿ�������ͷ��˶������Ϣ��
  
    Tau=(1-rho).*Tau + Delta_pheromone + Delta_pheromone_elitists;%����ѭ��������·���ϵ���Ϣ��
    
    iter = iter + 1;
    tabu=zeros(m,n);
 % ����������1�����·����¼��

 %   figure;
 %���·���ĵ����仯����
    [Shortest_Length,index] = min(Length_best(1:iter));
    Shortest_Route = Route_best(index,:);
    plot([citys(Shortest_Route,1);citys(Shortest_Route(1),1)],...
    [citys(Shortest_Route,2);citys(Shortest_Route(1),2)],'o-');
    pause(0.3);
 
    iter = iter + 1;
    Table = zeros(m,n);

 % end
end

%% �����ʾ
[Shortest_Length,index] = min(Length_best);
Shortest_Route = Route_best(index,:);
disp(['��̾���:' num2str(Shortest_Length)]);
disp(['���·��:' num2str([Shortest_Route Shortest_Route(1)])]);

%% ��ͼ
figure(1)
plot([citys(Shortest_Route,1);citys(Shortest_Route(1),1)],...
     [citys(Shortest_Route,2);citys(Shortest_Route(1),2)],'o-');
grid on
for i = 1:size(citys,1)
    text(citys(i,1),citys(i,2),['   ' num2str(i)]);
end
text(citys(Shortest_Route(1),1),citys(Shortest_Route(1),2),'       ���');
text(citys(Shortest_Route(end),1),citys(Shortest_Route(end),2),'       �յ�');
xlabel('����λ�ú�����')
ylabel('����λ��������')
title(['��Ⱥ�㷨�Ż�·��(��̾���:' num2str(Shortest_Length) ')'])
figure(2)
plot(1:iter_max,Length_best,'b',1:iter_max,Length_ave,'r:')
legend('��̾���','ƽ������')
xlabel('��������')
ylabel('����')
title('������̾�����ƽ������Ա�')